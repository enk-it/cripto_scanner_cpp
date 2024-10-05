//
// Created by silencer on 05.10.24.
//

#include "../../include/class/BybitImpl.h"
#include "../../include/utils/request.h"
#include "../../include/utils/shared.h"
// #include <cpr/cpr.h>

#include <iostream>
#include <nlohmann/json.hpp>
using namespace std::chrono_literals;


BybitImpl::BybitImpl() = default;

BybitImpl::BybitImpl(Scanner *scanner, string stockmarket_name) {
    this->scanner = scanner;
    this->stockmarket_name = stockmarket_name;
}

void BybitImpl::get_symbols_info() {
    // auto response = cpr::Get(cpr::Url{"https://api.bybit.com/v5/market/instruments-info?category=spot"});
    // string temp = response.text;
    // std::cout << temp << std::endl;
    // nlohmann::json jsonData;
    // jsonData = nlohmann::json::parse(temp);


}


void BybitImpl::init(net::io_context& ioc) {

}