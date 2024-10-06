#pragma once

#include <string>
#include <vector>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;



struct PathNode;

std::string get_uuid();

double count_fr(int n, double comission_k = 0.999);

double show_log_10(double n);

std::vector<PathNode *>* deep_copy(std::vector<PathNode *> *nodes);

net::awaitable<void> send_message(std::string message, websocket::stream<beast::ssl_stream<beast::tcp_stream> > &ws);


net::awaitable<void> connect_websocket(
        std::string host,
        std::string port,
        std::string target,
        websocket::stream<beast::ssl_stream<beast::tcp_stream> > **ws,
        ssl::context **ctx
    );

std::string httpsGet(const std::string& host, const std::string& path, ssl::context *ctx);


