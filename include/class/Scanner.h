//
// Created by silencer on 17.09.24.
//

#pragma once


#include <unordered_map>
#include <utility>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::unordered_map;


#include "../structure/Path.h"
#include "../structure/Symbol.h"


class Scanner {
protected:
    vector<string> tokens;
    unordered_map<string, Symbol *> symbols;
    vector<Path> paths;
    int maxlen;

public:
    Scanner();

    ~Scanner();

    Scanner(vector<string> t, int m);

    void update_symbol(string ticker, double best_ask_qty, double best_ask_price, double best_bid_qty,
                       double best_bid_price);

    void add_symbol(Symbol *new_symbol, string ticker);

    void scan_for_best_fr();

    bool is_allowed_token(const string &token);

    void print_symbols();
};



