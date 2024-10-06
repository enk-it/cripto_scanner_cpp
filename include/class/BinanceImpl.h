#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include "../../include/class/BaseCriptoStock.h"

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

    net::awaitable<void> subscribe();

    net::awaitable<void> read_stream_message();

    net::awaitable<void> read_api_message();

    net::awaitable<void> get_symbols_info();

    net::awaitable<void> init_stream_ws();

    net::awaitable<void> init_api_ws();

public:
    BinanceImpl();
    BinanceImpl(Scanner *scanner, string stockmarket_name);

    void init() override;


};



