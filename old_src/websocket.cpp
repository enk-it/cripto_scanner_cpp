#include "websocket.h"
#include <nlohmann/json.hpp>
using nlohmann::json;

void subscribe(const vector<Symbol>* symbols_vector,
               WebSocket* web_socket)
{
    unsigned long symbols_count = symbols_vector->size();
    vector<string> tokens_list;
    for (unsigned long i = 0; i < symbols_count; i++) {
        tokens_list.push_back(to_lower_copy(symbols_vector->at(i).symbol) + "@aggTrade");
    }

    json response = {
        {"method", "SUBSCRIBE"},
        {"params", tokens_list},
    };


    cout << response.dump() << endl;

    web_socket->send(response.dump());
}


void process_websocket_message(const ix::WebSocketMessagePtr& msg,
                               WebSocket* webSocket,
                               vector<Symbol>* symbols_vector
    )
{
    if (msg->type == ix::WebSocketMessageType::Message)
    {
        cout << "received message: " << msg->str << std::endl;
        cout << "> " << std::flush;
    }
    else if (msg->type == ix::WebSocketMessageType::Open)
    {
        cout << "Connection established" << std::endl;
        subscribe(symbols_vector, webSocket);
        cout << "> " << std::flush;
    }
    else if (msg->type == ix::WebSocketMessageType::Error)
    {
        // Maybe SSL is not configured properly
        cout << "Connection error: " << msg->errorInfo.reason << std::endl;
        cout << "> " << std::flush;
    }
    else if (msg->type == ix::WebSocketMessageType::Ping)
    {
        cout << "Ping" << std::endl;
        cout << "> " << std::flush;
    }
    else if (msg->type == ix::WebSocketMessageType::Pong)
    {
        cout << "Pong" << std::endl;
        cout << "> " << std::flush;
    }
}



WebSocket* connect_websocket(vector<Symbol>* symbols_vector)
{
    WebSocket webSocket;

    const string url("wss://stream.binance.com:443/ws");

    webSocket.setUrl(url);
    cout << "Settting callback" << endl;

    webSocket.setOnMessageCallback([&webSocket, symbols_vector](const ix::WebSocketMessagePtr& msg) {
            process_websocket_message(msg, &webSocket, symbols_vector);
        }
    );
    cout << "Starting" << endl;

    webSocket.start();

    webSocket.ping("ping");

    cout << "> " << std::flush;

    std::string text;

    while (std::getline(std::cin, text))
    {
        webSocket.send(text);
        cout << "> " << std::flush;
    }

    return &webSocket;

    // webSocket.send();

    // {"method":"SUBSCRIBE","params":["btcusdt@aggTrade"]}

    // Display a prompt


}