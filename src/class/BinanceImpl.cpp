//
// Created by silencer on 19.09.24.
//

#include "../../include/class/BinanceImpl.h"

#include <iostream>


net::awaitable<void> BinanceImpl::connect_stream_websocket() {
    const string host = "stream.binance.com";
    const string port = "443";


    auto executor = co_await net::this_coro::executor;
    auto resolver = net::ip::tcp::resolver{ executor };

    this->ctx = std::make_unique<ssl::context>(ssl::context::tlsv12_client);
    this->ctx->set_default_verify_paths();
    this->ctx->set_verify_mode(ssl::verify_peer);
    this->ws = std::make_unique<websocket::stream<beast::ssl_stream<beast::tcp_stream>>>(executor, *ctx);


    auto const results = co_await resolver.async_resolve(host, port);

    get_lowest_layer(*this->ws).expires_after(std::chrono::seconds(30));

    auto ep = co_await get_lowest_layer(*this->ws).async_connect(results);

    if(!SSL_set_tlsext_host_name(this->ws->next_layer().native_handle(), host.c_str()))
        throw beast::system_error(
            beast::error_code(
                static_cast<int>(::ERR_get_error()),
                net::error::get_ssl_category()),
            "Failed to set SNI Hostname");

    std::string full_host = host + ':' + std::to_string(ep.port());

    get_lowest_layer(*this->ws).expires_never();

    this->ws->set_option(
        websocket::stream_base::timeout::suggested(beast::role_type::client));

    this->ws->set_option(websocket::stream_base::decorator(
        [](websocket::request_type& req)
        {
            req.set(http::field::user_agent,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-coro");
        }));

    co_await this->ws->next_layer().async_handshake(ssl::stream_base::client);
    co_await this->ws->async_handshake(full_host, "/ws");

}


net::awaitable<void> BinanceImpl::send_stream_message(string message) {
    std::cout << "В начале send_message()" << std::endl;

    co_await this->ws->async_write(net::buffer(message));

    std::cout << "В конце send_message()" << std::endl;

}


net::awaitable<void> BinanceImpl::read_stream_message() {
    std::cout << "Вначале read_message()" << std::endl;
    beast::flat_buffer buffer;
    while (true) {
        std::cout << "Внутри read_message()" << std::endl;
        buffer.consume(buffer.size());
        co_await this->ws->async_read(buffer);
        string temp = beast::buffers_to_string(buffer.data());
        std::cout << temp << std::endl;
    }
    std::cout << "В конце read_message()" << std::endl;

}


net::awaitable<void> BinanceImpl::subscribe() {
    std::cout << "В начале subscribe()" << std::endl;

    co_await this->send_stream_message(R"({"method":"SUBSCRIBE","params":["btcusdt@bookTicker", "bnbeth@bookTicker"],"id":1727536128794})");
    std::cout << "В конце subscribe()" << std::endl;


}


net::awaitable<void> BinanceImpl::get_symbols_info() {
    std::cout << "В начале get_symbols_info()" << std::endl;
    co_await this->send_stream_message(R"({"id":"5494febb-d167-46a2-996d-70533eb4d976","method":"exchangeInfo","params":{}})");

    std::cout << "В конце get_symbols_info()" << std::endl;


}

BinanceImpl::BinanceImpl() = default;
BinanceImpl::BinanceImpl(Scanner* scanner) {
    this->scanner = scanner;
}


net::awaitable<void> BinanceImpl::init() {
    std::cout << "Внутри init()" << std::endl;

    co_await this->connect_stream_websocket();
    std::cout << "После connect()" << std::endl;

    // co_await this->get_symbols_info();
    // std::cout << "После get_symbols_info()" << std::endl;

    co_await this->subscribe();
    std::cout << "После subscribe()" << std::endl;

    co_await this->read_stream_message();
    std::cout << "После read_message()" << std::endl;

}

