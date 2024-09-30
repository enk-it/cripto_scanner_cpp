//
// Created by silencer on 17.09.24.
//

#include "../../include/class/Scanner.h"
#include "../../include/class/BaseCriptoStock.h"

#include <iostream>


Scanner::Scanner() : maxlen() {
}

Scanner::~Scanner() = default;

Scanner::Scanner(vector<string> t, int m) : tokens(std::move(t)), maxlen(m) {
}


void Scanner::update_symbol(string ticker, double best_ask_qty, double best_ask_price, double best_bid_qty,
                            double best_bid_price) {
    this->symbols[ticker]->bestAskQty = best_ask_qty;
    this->symbols[ticker]->bestAskPrice = best_ask_price;
}

void Scanner::add_symbol(Symbol *new_symbol, const string& ticker) {
    this->symbols[ticker] = new_symbol;
}

void Scanner::scan_for_best_fr() {
    int paths_len = this->paths.size();
    for (int i = 0; i < paths_len; i++) {
        if (this->paths[i].financial_result > 1) {
            std::cout << paths[i].financial_result << std::endl;
        }
    }
}

bool Scanner::is_allowed_token(const string &token) {
    for (int i = 0; i < this->tokens.size(); i++) {
        if (this->tokens[i] == token) return true;
    }
    return false;
}

void Scanner::print_symbols() {
    for (auto kv: this->symbols) {
        std::cout << kv.first + " ";
        std::cout << kv.second << std::endl;
    }
}
