#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <ixwebsocket/IXWebSocket.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "../include/structures.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using ix::WebSocket;
using boost::algorithm::to_lower_copy;

void subscribe(const vector<Symbol>* symbols_vector, WebSocket* web_socket);

void process_websocket_message(
    const ix::WebSocketMessagePtr& msg,
    WebSocket* webSocket,
    vector<Symbol>* symbols_vector
    );


WebSocket* connect_websocket(vector<Symbol>* symbols_vector);


#endif //WEBSOCKET_H
