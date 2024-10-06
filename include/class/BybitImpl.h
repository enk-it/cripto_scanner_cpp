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


class BybitImpl : public BaseCriptoStock {
protected:
    websocket::stream<beast::ssl_stream<beast::tcp_stream> > *stream_ws = nullptr;
    ssl::context *ctx = nullptr; // Также храним SSL-контекст в поле класса

    net::awaitable<void> subscribe();

    net::awaitable<void> read_stream_message();

    void get_symbols_info();

    net::awaitable<void> init_stream_ws();

    unordered_map<string, unordered_map<string, string>*> order_book_bid;
    unordered_map<string, unordered_map<string, string>*> order_book_ask;


public:
    BybitImpl();

    BybitImpl(Scanner *scanner,
              const string &stockmarket_name);

    void init() override;


};



