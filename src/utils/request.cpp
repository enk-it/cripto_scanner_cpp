#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include "../../include/utils/request.h"
#include "../../include/utils/shared.h"


std::string binance_subscribe_request(std::vector<std::string> symbols) {
    nlohmann::json request;

    for (int i = 0; i< symbols.size(); i++) {
        std::string symbol = symbols[i];
        std::ranges::transform(
            symbol, symbol.begin(),
              [](unsigned char c){ return std::tolower(c); });
        symbols[i] = symbol + "@bookTicker";
    }

    request["method"] = "SUBSCRIBE";
    request["params"] = symbols;


    return request.dump();

}

std::string binance_exchange_request() {
    nlohmann::json request;
    request["id"] = get_uuid();
    request["method"] = "exchangeInfo";
    request["params"] = nlohmann::json::object();


    return request.dump();
}


std::string bybit_subscribe_request(std::vector<std::string> symbols) {
    nlohmann::json request;

    for (int i = 0; i < symbols.size(); i++) {
        std::string symbol = symbols[i];
        symbols[i] = "orderbook.1." + symbol;
    }

    request["req_id"] = get_uuid();
    request["op"] = "subscribe";
    request["args"] = symbols;

    return request.dump();
}