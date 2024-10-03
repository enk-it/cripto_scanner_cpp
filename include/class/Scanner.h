//
// Created by silencer on 17.09.24.
//

#pragma once


#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <unordered_set>

using std::string;
using std::vector;
using std::unordered_map;


#include "../structure/Path.h"
#include "../structure/Symbol.h"


class Scanner {
protected:
    vector<string> tokens;
    unordered_map<string, Symbol *> symbols;

    // Key - Token; Values - Vector with Symbols with key Token
    unordered_map<string, vector<Symbol *> > sorted_symbols;

    vector<Path*> paths;
    int initialized_paths = 0;
    int maxlen;

    void _generate_paths(
        const string &start_token,
        const string *current_token,
        vector<Symbol *> &history,
        vector<PathNode *> *history_nodes
    );

    void _reduce_influence(const string& symbol);
    void _increase_influence(const string& symbol);

public:
    Scanner();

    ~Scanner();

    Scanner(vector<string> t, int m);

    void update_symbol(const string &ticker, double best_ask_qty, double best_ask_price, double best_bid_qty,
                       double best_bid_price);

    void add_symbol(Symbol *new_symbol, const string &ticker);

    void scan_for_best_fr();

    bool is_allowed_token(const string &token);

    void print_symbols();

    void generate_paths();

    void print_paths();

    void print_symbols_details();

    int get_paths_len();
};



