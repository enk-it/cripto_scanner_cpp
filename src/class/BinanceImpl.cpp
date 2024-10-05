//
// Created by silencer on 19.09.24.
//

#include "../../include/class/BinanceImpl.h"
#include "../../include/utils/request.h"

#include <iostream>
#include <nlohmann/json.hpp>
using namespace std::chrono_literals;


BinanceImpl::BinanceImpl() = default;

BinanceImpl::BinanceImpl(Scanner *scanner, string stockmarket_name) {
    this->scanner = scanner;
    this->stockmarket_name = stockmarket_name;
}

net::awaitable<void> BinanceImpl::read_stream_message() {
    beast::flat_buffer buffer;
    while (true) {
        buffer.consume(buffer.size());
        co_await this->stream_ws->async_read(buffer);
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
            this->scanner->update_symbol(
                this->stockmarket_name + symbol_name,
                ask_qty,
                ask_price,
                bid_qty,
                bid_price
            );
            //--------------------DEBUG-------------------------
            // std::cout << s << ' ' <<
            // std::format("{}", b) << ' ' <<
            // std::format("{}", B) << ' ' <<
            // std::format("{}", a) << ' ' <<
            // std::format("{}", A) << ' ' << std::endl;
            //--------------------DEBUG-------------------------
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
            this->scanner->add_symbol(symbol_, this->get_name() + sym);
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
            boost::asio::use_awaitable); // Асинхронно ждем 100 мс
    }

    string request = subscribe_request(this->symbols_names);
    co_await this->send_message(request, *this->stream_ws);
}


net::awaitable<void> BinanceImpl::get_symbols_info() {
    string request = exchange_request();

    co_await this->send_message(
        request,
        *this->api_ws);
}


net::awaitable<void> BinanceImpl::init_stream_ws() {
    co_await this->connect_websocket("stream.binance.com", "443", "/ws", &this->stream_ws, &this->ctx);
    co_await this->subscribe();
    co_await this->read_stream_message();
}


net::awaitable<void> BinanceImpl::init_api_ws() {
    co_await this->connect_websocket("ws-api.binance.com", "443", "/ws-api/v3", &this->api_ws, &this->ctx);
    co_await this->get_symbols_info();
    co_await this->read_api_message();

    this->scanner->generate_paths();
    // this->scanner->print_paths();

}


net::awaitable<void> BinanceImpl::init() {
    co_await this->init_api_ws();
}

