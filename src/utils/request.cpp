//
// Created by silencer on 01.10.24.
//

#include "../../include/utils/request.h"
#include "../../include/utils/shared.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>


std::string subscribe_request(std::vector<std::string> symbols) {
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

std::string exchange_request() {
    nlohmann::json request;
    request["id"] = get_uuid();
    request["method"] = "exchangeInfo";
    request["params"] = nlohmann::json::object();


    return request.dump();
}