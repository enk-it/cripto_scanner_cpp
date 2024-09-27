//
// Created by silencer on 17.09.24.
//

#include "../../include/class/Scanner.h"

#include <iostream>

#include "../../include/interface/ICriptoStock.h"


Scanner::Scanner() : maxlen() {}
Scanner::~Scanner() = default;
Scanner::Scanner(vector<string> t, int m) : tokens(std::move(t)), maxlen(m) {}


void Scanner::updateSymbol(string ticker, double best_ask_qty, double best_ask_price , double best_bid_qty, double best_bid_price) {
    this->symbols[ticker]->bestAskQty = best_ask_qty;
    this->symbols[ticker]->bestAskPrice = best_ask_price;
}

void Scanner::addSymbol(Symbol* new_symbol) {
    string ticker = new_symbol->criptostock->get_name() + new_symbol->symbol;
    this->symbols[ticker] = new_symbol;
}

void Scanner::scanForBestFinancialResult() {
    int paths_len = this->paths.size();
    for (int i =0; i < paths_len; i++) {
        if (this-> paths[i].financial_result > 1) {
            std::cout << paths[i].financial_result << std::endl;
        }
    }
}
