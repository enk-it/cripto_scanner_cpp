//
// Created by silencer on 05.10.24.
//

#include "../../include/class/BybitImpl.h"
#include "../../include/utils/request.h"
#include "../../include/utils/shared.h"
// #include <cpr/cpr.h>

#include <iostream>
#include <nlohmann/json.hpp>
using namespace std::chrono_literals;


BybitImpl::BybitImpl() = default;

BybitImpl::BybitImpl(Scanner *scanner, const string &stockmarket_name) {
    this->scanner = scanner;
    this->stockmarket_name = stockmarket_name;

    this->ctx = new ssl::context(ssl::context::tlsv12_client);
    this->ctx->set_default_verify_paths();
    this->ctx->set_verify_mode(ssl::verify_peer);
}

void BybitImpl::get_symbols_info() {
    string temp = httpsGet("api.bybit.com", "/v5/market/instruments-info?category=spot", this->ctx);
    nlohmann::json jsonData;
    jsonData = nlohmann::json::parse(temp);

    for (int i = 0; i < jsonData["result"]["list"].size(); i++) {
        std::string sym = jsonData["result"]["list"][i]["symbol"].get<std::string>();
        std::string baseAsset = jsonData["result"]["list"][i]["baseCoin"].get<std::string>();
        std::string quoteAsset = jsonData["result"]["list"][i]["quoteCoin"].get<std::string>();
        std::string status = jsonData["result"]["list"][i]["status"].get<std::string>();
        if (status != "Trading") {
            continue;
        }
        if (this->scanner->is_allowed_token(baseAsset) && this->scanner->is_allowed_token(quoteAsset)) {
            Symbol *symbol_ = new Symbol;
            symbol_->symbol = sym;
            symbol_->base = baseAsset;
            symbol_->quote = quoteAsset;
            symbol_->bestAskPrice = 1;
            symbol_->bestBidPrice = 1;
            symbol_->bestBidQty = 1;
            symbol_->bestAskQty = 1;
            symbol_->criptostock = this;
            this->scanner->add_symbol(symbol_, this->stockmarket_name + sym);
            this->symbols_names.push_back(sym);
            this->order_book_bid[sym] = new unordered_map<std::string, std::string>;
            this->order_book_ask[sym] = new unordered_map<std::string, std::string>;
        }
    }
}


net::awaitable<void> BybitImpl::subscribe() {
    auto executor = co_await net::this_coro::executor;
    net::steady_timer timer(executor);

    vector<string> temp_vector;
    temp_vector.reserve(this->symbols_names.size());
    string request;

    for (int i = 0; i < this->symbols_names.size(); i++) {
        temp_vector.push_back(this->symbols_names[i]);

        if (temp_vector.size() == 10 || i == this->symbols_names.size() - 1) {
            request = bybit_subscribe_request(temp_vector);
            co_await send_message(request, *this->stream_ws);
            temp_vector.clear();
            std::cout << request << std::endl;
        }
    }
}

net::awaitable<void> BybitImpl::read_stream_message() {
    beast::flat_buffer buffer;
    while (true) {
        buffer.consume(buffer.size());
        co_await this->stream_ws->async_read(buffer);

        if (!this->to_update) {
            continue;
        }

        string temp = beast::buffers_to_string(buffer.data());

        // std::cout << temp << std::endl;

        nlohmann::json jsonData;
        jsonData = nlohmann::json::parse(temp);

        if (jsonData.contains("topic")) {
            std::string update_type = jsonData["type"].get<std::string>();
            std::string symbol_name = jsonData["data"]["s"].get<std::string>();

            vector<vector<std::string>> asks = jsonData["data"]["a"].get<vector<vector<std::string>>>();
            vector<vector<std::string>> bids = jsonData["data"]["b"].get<vector<vector<std::string>>>();

            if (update_type == "snapshot") {
                this->order_book_bid[symbol_name]->clear();
                this->order_book_ask[symbol_name]->clear();

                std::string bid_price = bids[0][0];
                std::string bid_qty = bids[0][1];
                std::string ask_price = asks[0][0];
                std::string ask_qty = asks[0][1];

                (*this->order_book_bid[symbol_name])[bid_price] = bid_qty;
                (*this->order_book_ask[symbol_name])[ask_price] = ask_qty;
            }
            else {
                if (asks.size() == 1) {
                    std::string ask_price = asks[0][0];
                    std::string ask_qty = asks[0][1];

                    if (ask_qty == "0") {
                        this->order_book_ask[symbol_name]->erase(ask_price);
                    }
                    else {
                        (*this->order_book_ask[symbol_name])[ask_price] = ask_qty;
                    }
                }
                if (bids.size() == 1) {
                    std::string bid_price = bids[0][0];
                    std::string bid_qty = bids[0][1];

                    if (bid_qty == "0") {
                        this->order_book_bid[symbol_name]->erase(bid_price);
                    }
                    else {
                        (*this->order_book_bid[symbol_name])[bid_price] = bid_qty;
                    }
                }
            }


            double bid_price = -1;
            double bid_qty = -1;
            double ask_price = -1;
            double ask_qty = -1;

            for (auto pair : *this->order_book_ask[symbol_name]) {
                double temp_first = std::stod(pair.first);
                double temp_second = std::stod(pair.second);
                if (ask_price == -1 || temp_first < ask_price) {
                    ask_price = temp_first;
                    ask_qty = temp_second;
                }
            }
            for (auto pair : *this->order_book_bid[symbol_name]) {
                double temp_first = std::stod(pair.first);
                double temp_second = std::stod(pair.second);
                if (bid_price == -1 || temp_first > bid_price) {
                    bid_price = temp_first;
                    bid_qty = temp_second;
                }
            }
            this->on_update(
                    symbol_name,
                    ask_qty,
                    ask_price,
                    bid_qty,
                    bid_price,
                    std::time(nullptr)
                );
        }
    }
}



net::awaitable<void> BybitImpl::init_stream_ws() {
    co_await connect_websocket("stream.bybit.com", "443", "/v5/public/spot", &this->stream_ws, &this->ctx);
    co_await this->subscribe();
    co_await this->read_stream_message();
}


void BybitImpl::init() {
    get_symbols_info();
    this->scanner->stock_ready();

    for (int i = 0; i < this->symbols_names.size(); i++) {
        std::cout << this->symbols_names[i] << std::endl;
    }

    net::co_spawn(
            *this->scanner->ioc,
            this->init_stream_ws(),
            [](std::exception_ptr e) {
                if (e)
                    std::rethrow_exception(e);
            });

    // net::co_spawn(
    //         *this->scanner->ioc,
    //         this->read_stream_message(),
    //         [](std::exception_ptr e) {
    //             if (e)
    //                 std::rethrow_exception(e);
    //         });

    // this->scanner->ioc->run();


}
