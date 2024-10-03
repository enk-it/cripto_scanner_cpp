//
// Created by silencer on 17.09.24.
//

#include "../../include/class/Scanner.h"

#include <cmath>
#include <format>

#include "../../include/class/BaseCriptoStock.h"
#include "../../include/structure/PathNode.h"
#include "../../include/structure/Path.h"
#include "../../include/utils/shared.h"

#include <iostream>


Scanner::Scanner() : maxlen() {
}

Scanner::~Scanner() = default;

Scanner::Scanner(vector<string> t, int m) : tokens(std::move(t)), maxlen(m) {
}


void Scanner::update_symbol(
    const string &ticker,
    const double best_ask_qty,
    const double best_ask_price,
    const double best_bid_qty,
    const double best_bid_price
    ) {

    this->_reduce_influence(ticker);

    // this->symbols[ticker]->bestAskQty = best_ask_qty;
    // this->symbols[ticker]->bestAskPrice = log10(best_ask_price);
    // this->symbols[ticker]->bestBidQty = best_bid_qty;
    // this->symbols[ticker]->bestBidPrice = log10(best_bid_price);

    this->symbols[ticker]->bestAskQty = best_ask_qty;
    this->symbols[ticker]->bestAskPrice = best_ask_price;
    this->symbols[ticker]->bestBidQty = best_bid_qty;
    this->symbols[ticker]->bestBidPrice = best_bid_price;
    this->symbols[ticker]->initialized = true;

    this->_increase_influence(ticker);

    system("clear");
    std::cout << std::time(nullptr) << std::endl;
    // this->print_symbols_details();
    std::cout << this->paths.size() << " " << this->initialized_paths << std::endl;
    this->scan_for_best_fr();
}


void Scanner::add_symbol(Symbol *new_symbol, const string &ticker) {
    this->symbols[ticker] = new_symbol;
    this->sorted_symbols[new_symbol->base].push_back(new_symbol);
    this->sorted_symbols[new_symbol->quote].push_back(new_symbol);
}


void Scanner::scan_for_best_fr() {
    for (int i = 0; i < this->paths.size(); i++) {
        bool path_initialized = true;
        if (this->paths[i]->initialized == false) {
            for (int j = 0; j < this->paths[i]->path.size(); j++) {
                if (this->paths[i]->path[j]->symbol->initialized == false) {
                    path_initialized = false;
                    break;
                }
            }
        }
        if (path_initialized == true) {
            this->paths[i]->initialized = true;
            this->initialized_paths += 1;
        }
        else {
            continue;
        }


        if (this->paths[i]->financial_result > 1) {
            for (int j = 0; j < this->paths[i]->path.size(); j++) {
                std::cout << this->paths[i]->path[j]->symbol->symbol << " ";
            }
            std::cout << this->paths[i]->financial_result << std::endl;
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
    const string &start_token,
    const string *current_token,
    vector<Symbol *> &history,
    vector<PathNode *> *history_nodes
) {
    for (Symbol *sym: this->sorted_symbols[*current_token]) {
        if (std::ranges::find(history, sym) != history.end()) continue;
        if (history_nodes->size() + 1 == this->maxlen && !(sym->base == start_token || sym->quote == start_token))
            continue;

        PathNode *path_node = new PathNode();
        path_node->symbol = sym;

        string new_token;
        if (sym->base == *current_token) {
            path_node->is_reversed = false;
            new_token = sym->quote;
        } else {
            path_node->is_reversed = true;
            new_token = sym->base;
        }

        history.push_back(sym);
        history_nodes->push_back(path_node);

        if (new_token == start_token) {
            Path *path = new Path(0, *deep_copy(history_nodes));

            for (int i = 0; i < path->path.size(); i++) {
                path->path[i]->path = path;
                path->path[i]->symbol->participates.push_back(path->path[i]);
            }
            path->financial_result = count_fr(history_nodes->size());
            paths.push_back(path);
        } else {
            _generate_paths(start_token, &new_token, history, history_nodes);
        }

        history.pop_back();
        history_nodes->pop_back();

    }
}


void Scanner::print_symbols_details() {
    for (const auto &[fst, snd]: this->symbols) {
        std::cout << snd->symbol << " ";
        std::cout << std::format("{}", snd->bestAskPrice) << " ";
        std::cout << std::format("{}", snd->bestBidPrice);
        std::cout << std::endl;
    }
}


void Scanner::generate_paths() {
    vector<Symbol *> history;
    vector<PathNode *> history_nodes;

    const string start_token = "USDT";

    this->_generate_paths(
        start_token,
        &start_token,
        history,
        &history_nodes
    );
}


void Scanner::print_paths() {
    for (int j = 0; j < this->paths.size(); j++) {
        std::cout << "--------------------" << std::endl;
        std::cout << this->paths[j]->financial_result << std::endl;
        for (int i = 0; i < this->paths[j]->path.size(); i++) {
            std::cout << this->paths[j]->path[i]->symbol->symbol << std::endl;
        }
    }
    std::cout << this->paths.size() << std::endl;
}


int Scanner::get_paths_len() {
    return this->paths.size();
}


void Scanner::_reduce_influence(const string& symbol) {
    Symbol* current_symbol = this->symbols[symbol];

    for (int i =0; i < current_symbol->participates.size(); i++) {
        if (current_symbol->participates[i]->is_reversed) {
            current_symbol->participates[i]->path->financial_result *= current_symbol->bestAskPrice; // возможно поменять местами с 187
        }
        else {
            current_symbol->participates[i]->path->financial_result /= current_symbol->bestBidPrice;
        }
    }
}

void Scanner::_increase_influence(const string& symbol) {
    Symbol* current_symbol = this->symbols[symbol];
    for (int i = 0; i < current_symbol->participates.size(); i++) {
        if (current_symbol->participates[i]->is_reversed) {
            current_symbol->participates[i]->path->financial_result /= current_symbol->bestAskPrice; // возможно поменять местами с 199
        }
        else {
            current_symbol->participates[i]->path->financial_result *= current_symbol->bestBidPrice;
        }
    }
}