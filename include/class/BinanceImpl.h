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

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;


class BinanceImpl : public BaseCriptoStock {
protected:
    std::unique_ptr<websocket::stream<beast::ssl_stream<beast::tcp_stream>>> ws;  // Хранение в умном указателе
    std::unique_ptr<ssl::context> ctx;  // Также храним SSL-контекст в поле класса

    net::awaitable<void> subscribe();
    net::awaitable<void> connect_websocket();
    net::awaitable<void> send_message(string message);
    net::awaitable<void> read_message();

public:
    BinanceImpl();
    BinanceImpl(Scanner* scanner);

    net::awaitable<void> init();



};



