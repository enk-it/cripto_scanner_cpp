//
// Created by silencer on 19.09.24.
//

#include "../../include/class/BinanceImpl.h"

#include <iostream>
#include <nlohmann/json.hpp>


BinanceImpl::BinanceImpl() = default;

BinanceImpl::BinanceImpl(Scanner *scanner, string stockmarket_name) {
    this->scanner = scanner;
    this->stockmarket_name = stockmarket_name;
}


net::awaitable<void> BinanceImpl::connect_websocket(
    const string host,
    const string port,
    const string target,
    websocket::stream<beast::ssl_stream<beast::tcp_stream> > **ws,
    ssl::context **ctx
) {
    auto executor = co_await net::this_coro::executor;
    auto resolver = net::ip::tcp::resolver{executor};

    (*ctx) = new ssl::context(ssl::context::tlsv12_client);
    (*ctx)->set_default_verify_paths();
    (*ctx)->set_verify_mode(ssl::verify_peer);
    (*ws) = new websocket::stream<beast::ssl_stream<beast::tcp_stream> >(executor, **ctx);

    auto const results = co_await resolver.async_resolve(host, port);

    get_lowest_layer(**ws).expires_after(std::chrono::seconds(30));

    auto ep = co_await get_lowest_layer(**ws).async_connect(results);

    if (!SSL_set_tlsext_host_name((*ws)->next_layer().native_handle(), host.c_str()))
        throw beast::system_error(
            beast::error_code(
                static_cast<int>(::ERR_get_error()),
                net::error::get_ssl_category()),
            "Failed to set SNI Hostname");

    std::string full_host = host + ':' + std::to_string(ep.port());

    get_lowest_layer(**ws).expires_never();

    (*ws)->set_option(
        websocket::stream_base::timeout::suggested(beast::role_type::client));

    (*ws)->set_option(websocket::stream_base::decorator(
        [](websocket::request_type &req) {
            req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-coro");
        }));

    co_await (*ws)->next_layer().async_handshake(ssl::stream_base::client);
    co_await (*ws)->async_handshake(full_host, target);
}


net::awaitable<void> BinanceImpl::send_message(string message,
                                               websocket::stream<beast::ssl_stream<beast::tcp_stream> > &ws) {
    std::cout << "В начале send_message()" << std::endl;
    co_await ws.async_write(net::buffer(message));
    std::cout << "В конце send_message()" << std::endl;
}


net::awaitable<void> BinanceImpl::read_stream_message() {
    std::cout << "Вначале read_stream_message()" << std::endl;
    beast::flat_buffer buffer;
    while (true) {
        std::cout << "Внутри read_stream_message()" << std::endl;
        buffer.consume(buffer.size());
        co_await this->stream_ws->async_read(buffer);
        string temp = beast::buffers_to_string(buffer.data());
        std::cout << temp << std::endl;
    }
    std::cout << "В конце read_stream_message()" << std::endl;
}


net::awaitable<void> BinanceImpl::read_api_message() {
    std::cout << "Вначале read_api_message()" << std::endl;
    beast::flat_buffer buffer;
    std::cout << "Внутри read_api_message()" << std::endl;
    buffer.consume(buffer.size());
    co_await this->api_ws->async_read(buffer);
    string temp = beast::buffers_to_string(buffer.data());
    nlohmann::json jsonData;
    jsonData = nlohmann::json::parse(temp);
    for (int i = 0; i < jsonData["result"]["symbols"].size(); i++) {
        std::string sym = jsonData["result"]["symbols"][i]["symbol"].get<std::string>();
        std::string baseAsset = jsonData["result"]["symbols"][i]["baseAsset"].get<std::string>();
        std::string quoteAsset = jsonData["result"]["symbols"][i]["quoteAsset"].get<std::string>();
        if (this->scanner->is_allowed_token(baseAsset) && this->scanner->is_allowed_token(quoteAsset)) {
            Symbol *symbol_ = new Symbol;
            symbol_->symbol = sym;
            symbol_->base = baseAsset;
            symbol_->quote = quoteAsset;
            this->scanner->add_symbol(symbol_, this->get_name() + sym);
        }
    }
    std::cout << "В конце read_api_message()" << std::endl;
}


net::awaitable<void> BinanceImpl::subscribe() {
    std::cout << "В начале subscribe()" << std::endl;
    co_await this->send_message(
        R"({"method":"SUBSCRIBE","params":["btcusdt@bookTicker", "bnbeth@bookTicker"],"id":1727536128794})",
        *this->stream_ws);
    std::cout << "В конце subscribe()" << std::endl;
}


net::awaitable<void> BinanceImpl::get_symbols_info() {
    std::cout << "В начале get_symbols_info()" << std::endl;
    co_await this->send_message(
        R"({"id":"5494febb-d167-46a2-996d-70533eb4d976","method":"exchangeInfo","params":{}})",
        *this->api_ws);
    std::cout << "В конце get_symbols_info()" << std::endl;
}


net::awaitable<void> BinanceImpl::init_stream_ws() {
    std::cout << "Внутри init_stream_ws()" << std::endl;
    co_await this->connect_websocket("stream.binance.com", "443", "/ws", &this->stream_ws, &this->ctx);
    std::cout << "После connect()" << std::endl;
    co_await this->subscribe();
    std::cout << "После subscribe()" << std::endl;
    co_await this->read_stream_message();
    std::cout << "После read_stream_message()" << std::endl;
}


net::awaitable<void> BinanceImpl::init_api_ws() {
    std::cout << "Внутри init_api_ws()" << std::endl;
    co_await this->connect_websocket("ws-api.binance.com", "443", "/ws-api/v3", &this->api_ws, &this->ctx);
    std::cout << "После connect()" << std::endl;
    co_await this->get_symbols_info();
    std::cout << "После get_symbols_info()" << std::endl;
    co_await this->read_api_message();
    std::cout << "После read_api_message()" << std::endl;

    this->scanner->print_symbols();
}

