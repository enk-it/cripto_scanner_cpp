//
// Created by silencer on 29.09.24.
//

#include "../../include/utils/shared.h"
#include "../../include/structure/PathNode.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.


struct PathNode;

std::string get_uuid() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return to_string(uuid);
}

double count_fr(int n, double comission_k) {
    // return log10(comission_k) * n;
    return pow(comission_k, n);
}


std::vector<PathNode*>* deep_copy(std::vector<PathNode *> *nodes) {
    std::vector<PathNode *>* result = new std::vector<PathNode *>({});

    for (int i = 0; i < nodes->size(); i++) {
        PathNode *node_copy = new PathNode();
        node_copy->symbol = nodes->at(i)->symbol;
        node_copy->is_reversed = nodes->at(i)->is_reversed;
        result->push_back(node_copy);
    }

    return result;
}


net::awaitable<void> connect_websocket(
    const std::string host,
    const std::string port,
    const std::string target,
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

net::awaitable<void> send_message(
    std::string message,
    websocket::stream<beast::ssl_stream<beast::tcp_stream> > &ws
) {
    co_await ws.async_write(net::buffer(message));
}
