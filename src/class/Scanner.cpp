//
// Created by silencer on 17.09.24.
//

#include "../../include/class/Scanner.h"
#include "../../include/class/BaseCriptoStock.h"
#include "../../include/structure/PathNode.h"
#include "../../include/structure/Path.h"
#include "../../include/utils/shared.h"

#include <iostream>
#include <unordered_set>


Scanner::Scanner() : maxlen() {
}

Scanner::~Scanner() = default;

Scanner::Scanner(vector<string> t, int m) : tokens(std::move(t)), maxlen(m) {
}


void Scanner::update_symbol(const string& ticker, double best_ask_qty, double best_ask_price, double best_bid_qty,
                            double best_bid_price) {
    this->symbols[ticker]->bestAskQty = best_ask_qty;
    this->symbols[ticker]->bestAskPrice = best_ask_price;
    this->symbols[ticker]->bestBidQty = best_bid_qty;
    this->symbols[ticker]->bestBidPrice = best_bid_price;
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


void Scanner::_generate_paths(
    const string* start_token,
    const string* current_token,
    std::unordered_set<string>* history_set,
    vector<PathNode*>* history
    )
{
    if (!history->empty() && *start_token == *current_token) {
        double financial_result = count_fr(0.999f, history->size());


        Path *new_path = new Path;

        new_path->financial_result = financial_result;
        new_path->path = *history;

        this->paths.push_back(*new_path);

        return;
    }
    if (history_set->size() >= this->maxlen) {
        return;
    }

    for (const auto& symbol : this->symbols_vec) {
        if (history_set->contains(symbol->symbol)) {
            continue;
        }
        if (symbol->base != *current_token && symbol->quote != *current_token) {
            continue;
        }
        string new_token;
        PathNode* new_path_node = new PathNode();
        new_path_node->symbol = symbol;

        if (symbol->base == *current_token) {
            new_token = symbol->quote;
            new_path_node->is_reversed = false;
        } else {
            new_token = symbol->base;
            new_path_node->is_reversed = true;
        }

        if (new_token != *start_token && history_set->contains(new_token)) {
            history->pop_back();
            continue;
        }

        history->push_back(new_path_node);
        history_set->insert(symbol->symbol);

        // proceed

        _generate_paths(
            start_token,
            &new_token,
            history_set,
            history
            );

        history->pop_back();
        history_set->erase(symbol->symbol);
    }
}


void Scanner::_vectorize_symbols() {
    int i = 0;
    for (const auto&[fst, snd]: this->symbols) {
        this->symbols_vec.push_back(snd);
        i++;
    }
}


void Scanner::generate_paths() {
    this->_vectorize_symbols();

    std::unordered_set<string> history_set;
    vector<PathNode*> history;

    const string start_token = "USDT";

    this->_generate_paths(
        &start_token,
        &start_token,
        &history_set,
        &history
    );


}


void Scanner::print_paths() {
    for (int j = 0; j < this->paths.size(); j++) {
        std::cout << "--------------------" << std::endl;
        std::cout << this->paths[j].financial_result << std::endl;
        for (int i = 0; i<this->paths[j].path.size(); i++) {
            std::cout << this->paths[j].path[i]->symbol->symbol << std::endl;
        }
    }
    std::cout << this->paths.size() << std::endl;
}

