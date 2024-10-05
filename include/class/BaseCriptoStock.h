//
// Created by silencer on 27.09.24.
//

#pragma once

#include "../../include/structure/Symbol.h"
#include "../../include/class/Scanner.h"


#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


using std::unordered_map;
using std::unordered_map;
using std::vector;
using std::string;
using std::move;


class BaseCriptoStock {
protected:
    Scanner* scanner;
    bool to_update=false;
    std::vector<string> symbols_names;

    void on_update (
        const string& symbol_name,
        double ask_qty,
        double ask_price,
        double bid_qty,
        double bid_price,
        long lud
        );

public:
    string stockmarket_name;

    BaseCriptoStock();
    explicit BaseCriptoStock(Scanner* scanner);
    ~BaseCriptoStock();
    virtual void init();
    void trade ();
    void allow_update();
};


