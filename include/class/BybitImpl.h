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


class BybitImpl : public BaseCriptoStock {
protected:
    websocket::stream<beast::ssl_stream<beast::tcp_stream> > *ws = nullptr;
    ssl::context *ctx = nullptr; // Также храним SSL-контекст в поле класса

    bool is_stopped = true;

    net::awaitable<void> subscribe();

    net::awaitable<void> read_stream_message();

    void get_symbols_info();

    net::awaitable<void> init_ws();


public:
    BybitImpl();

    BybitImpl(Scanner *scanner,
              const string &stockmarket_name);

    void init(net::io_context& ioc);


};



