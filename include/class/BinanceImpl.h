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
    websocket::stream<beast::ssl_stream<beast::tcp_stream> > *stream_ws = nullptr;
    websocket::stream<beast::ssl_stream<beast::tcp_stream> > *api_ws = nullptr;
    ssl::context *ctx = nullptr; // Также храним SSL-контекст в поле класса

    bool is_stopped = true;

    std::vector<string> symbols_names;

    net::awaitable<void> subscribe();

    net::awaitable<void> connect_websocket(
        string host,
        string port,
        string target,
        websocket::stream<beast::ssl_stream<beast::tcp_stream> > **ws,
        ssl::context **ctx
    );

    net::awaitable<void> send_message(string message, websocket::stream<beast::ssl_stream<beast::tcp_stream> > &ws);

    net::awaitable<void> read_stream_message();

    net::awaitable<void> read_api_message();

    net::awaitable<void> get_symbols_info();

public:
    BinanceImpl();

    BinanceImpl(Scanner *scanner, string stockmarket_name);

    net::awaitable<void> init_stream_ws();

    net::awaitable<void> init_api_ws();

    net::awaitable<void> init();


};



