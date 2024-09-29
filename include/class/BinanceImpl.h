//
// Created by silencer on 19.09.24.
//

#pragma once
#include "../../include/class/BaseCriptoStock.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;


class BinanceImpl : public BaseCriptoStock {
protected:
    websocket::stream<beast::ssl_stream<beast::tcp_stream>>* ws = nullptr;  // Хранение в умном указателе
    ssl::context* ctx = nullptr;  // Также храним SSL-контекст в поле класса

    net::awaitable<void> subscribe();
    net::awaitable<void> connect_stream_websocket(
        string host,
        string port,
        string target,
        websocket::stream<beast::ssl_stream<beast::tcp_stream>> **ws,
        ssl::context **ctx
        );
    net::awaitable<void> send_stream_message(string message);
    net::awaitable<void> read_stream_message();
    net::awaitable<void> get_symbols_info();

public:
    BinanceImpl();
    BinanceImpl(Scanner* scanner);

    net::awaitable<void> init();



};



