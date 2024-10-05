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

BybitImpl::BybitImpl(Scanner *scanner,
                     const string &stockmarket_name) {
    this->scanner = scanner;
    this->stockmarket_name = stockmarket_name;

    this->ctx = new ssl::context(ssl::context::tlsv12_client);
    this->ctx->set_default_verify_paths();
    this->ctx->set_verify_mode(ssl::verify_peer);
}

void BybitImpl::get_symbols_info() {
    string temp = httpsGet("api.bybit.com", "/v5/market/instruments-info?category=spot", this->ctx);
    std::cout << temp << std::endl;
    nlohmann::json jsonData;
    jsonData = nlohmann::json::parse(temp);

    for (int i = 0; i < jsonData["result"]["list"].size(); i++) {
        std::string sym = jsonData["result"]["list"][i]["symbol"].get<std::string>();
        std::string baseAsset = jsonData["result"]["list"][i]["baseCoin"].get<std::string>();
        std::string quoteAsset = jsonData["result"]["list"][i]["quoteCoin"].get<std::string>();
        std::string status = jsonData["result"]["list"][i]["status"].get<std::string>();
        if (status != "Trading") {
            continue;
        }
        if (this->scanner->is_allowed_token(baseAsset) && this->scanner->is_allowed_token(quoteAsset)) {
            Symbol *symbol_ = new Symbol;
            symbol_->symbol = sym;
            symbol_->base = baseAsset;
            symbol_->quote = quoteAsset;
            symbol_->bestAskPrice = 1;
            symbol_->bestBidPrice = 1;
            symbol_->bestBidQty = 1;
            symbol_->bestAskQty = 1;
            symbol_->criptostock = this;
            this->scanner->add_symbol(symbol_, this->stockmarket_name + sym);
            this->symbols_names.push_back(sym);
        }
    }



}


void BybitImpl::init(net::io_context& ioc) {
    get_symbols_info();
}