//
// Created by silencer on 01.10.24.
//

#ifndef REQUEST_H
#define REQUEST_H
#include <string>
#include <vector>

std::string binance_subscribe_request(std::vector<std::string> symbols);
std::string binance_exchange_request();

std::string bybit_subscribe_request(std::vector<std::string> symbols);


#endif //REQUEST_H
