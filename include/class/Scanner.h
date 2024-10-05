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

#include <boost/asio/co_spawn.hpp>


class Scanner {
protected:
    vector<string> tokens;
    unordered_map<string, Symbol *> symbols;
    vector<BaseCriptoStock*> stocks;
    unordered_map<string, vector<Symbol *> > sorted_symbols;
    vector<Path*> paths;
    int initialized_paths = 0;
    int maxlen;

    int stocks_ready = 0;

    void _generate_paths(
        const string &start_token,
        const string *current_token,
        vector<Symbol *> &history,
        vector<PathNode *> *history_nodes
    );

    void _reduce_influence(const string& symbol);
    void _increase_influence(const string& symbol);

    void scan_for_best_fr();

    void print_symbols_details();
    void print_symbols();
    void generate_paths();

public:
    Scanner();
    ~Scanner();
    Scanner(vector<string> t, int m, boost::asio::io_context* ioc);

    boost::asio::io_context* ioc = nullptr;


    void init();
    void stock_ready(); // Биржа обязана это использовать
    void update_symbol(const string &ticker, double best_ask_qty, double best_ask_price, double best_bid_qty,
                       double best_bid_price); // Биржа обязана это использовать
    void add_symbol(Symbol *new_symbol, const string &ticker); // Биржа обязана это использовать
    void add_stock(BaseCriptoStock* stock); // Обязательно использовать для добавления
    bool is_allowed_token(const string &token); // Биржа обязана это использовать
};



