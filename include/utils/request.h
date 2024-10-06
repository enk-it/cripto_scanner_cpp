#pragma once

#include <string>
#include <vector>

std::string binance_subscribe_request(std::vector<std::string> symbols);
std::string binance_exchange_request();

std::string bybit_subscribe_request(std::vector<std::string> symbols);

