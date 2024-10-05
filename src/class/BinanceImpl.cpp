//
// Created by silencer on 19.09.24.
//

#include "../../include/class/BinanceImpl.h"
#include "../../include/utils/request.h"
#include "../../include/utils/shared.h"

#include <iostream>
#include <nlohmann/json.hpp>
using namespace std::chrono_literals;


BinanceImpl::BinanceImpl() = default;

BinanceImpl::BinanceImpl(Scanner *scanner, string stockmarket_name) {
    this->scanner = scanner;
    this->stockmarket_name = stockmarket_name;

    this->ctx = new ssl::context(ssl::context::tlsv12_client);
    this->ctx->set_default_verify_paths();
    this->ctx->set_verify_mode(ssl::verify_peer);
}

net::awaitable<void> BinanceImpl::read_stream_message() {
    beast::flat_buffer buffer;
    while (true) {
        buffer.consume(buffer.size());
        co_await this->stream_ws->async_read(buffer);

        if (!this->to_update) {
            continue;
        }

        string temp = beast::buffers_to_string(buffer.data());
        nlohmann::json jsonData;
        jsonData = nlohmann::json::parse(temp);

        if (!jsonData.contains("result")) {
            // int timestamp = jsonData["u"].get<int>();
            std::string symbol_name = jsonData["s"].get<std::string>();
            double bid_price = std::stod(jsonData["b"].get<std::string>());
            double bid_qty = std::stod(jsonData["B"].get<std::string>());
            double ask_price = std::stod(jsonData["a"].get<std::string>());
            double ask_qty = std::stod(jsonData["A"].get<std::string>());

            this->on_update(
                    symbol_name,
                    ask_qty,
                    ask_price,
                    bid_qty,
                    bid_price,
                    std::time(nullptr)
                );
        } else {
            std::cout << "Статус подписки на стрим" << std::endl;
        }
    }
}


net::awaitable<void> BinanceImpl::read_api_message() {
    beast::flat_buffer buffer;
    buffer.consume(buffer.size());
    co_await this->api_ws->async_read(buffer);
    string temp = beast::buffers_to_string(buffer.data());
    nlohmann::json jsonData;
    jsonData = nlohmann::json::parse(temp);
    for (int i = 0; i < jsonData["result"]["symbols"].size(); i++) {
        std::string sym = jsonData["result"]["symbols"][i]["symbol"].get<std::string>();
        std::string baseAsset = jsonData["result"]["symbols"][i]["baseAsset"].get<std::string>();
        std::string quoteAsset = jsonData["result"]["symbols"][i]["quoteAsset"].get<std::string>();
        std::string status = jsonData["result"]["symbols"][i]["status"].get<std::string>();
        if (status != "TRADING") {
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
        }
    }

    this->is_stopped = false;
}


net::awaitable<void> BinanceImpl::subscribe() {
    auto executor = co_await net::this_coro::executor;
    net::steady_timer timer(executor);

    while (this->is_stopped) {
        timer.expires_after(100ms); // Устанавливаем таймер на 100 мс
        co_await timer.async_wait(
            boost::asio::use_awaitable
            ); // Асинхронно ждем 100 мс
    }

    string request = binance_subscribe_request(this->symbols_names);
    co_await send_message(request, *this->stream_ws);
}


net::awaitable<void> BinanceImpl::get_symbols_info() {
    string request = binance_exchange_request();

    co_await send_message(
        request,
        *this->api_ws);
}


net::awaitable<void> BinanceImpl::init_stream_ws() {
    co_await connect_websocket("stream.binance.com", "443", "/ws", &this->stream_ws, &this->ctx);
    co_await this->subscribe();
    co_await this->read_stream_message();
}


net::awaitable<void> BinanceImpl::init_api_ws() {
    co_await connect_websocket("ws-api.binance.com", "443", "/ws-api/v3", &this->api_ws, &this->ctx);
    co_await this->get_symbols_info();
    co_await this->read_api_message();

    this->scanner->stock_ready();
}


void BinanceImpl::init() {
    net::co_spawn(
            *this->scanner->ioc,
            this->init_stream_ws(),
            [](std::exception_ptr e) {
                if (e)
                    std::rethrow_exception(e);
            });

    net::co_spawn(
        *this->scanner->ioc,
        this->init_api_ws(),
        [](std::exception_ptr e) {
            if (e)
                std::rethrow_exception(e);
        });
}

