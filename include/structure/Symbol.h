//
// Created by silencer on 16.09.24.
//

#pragma once

#include <string>
#include <vector>

class BaseCriptoStock;
using std::string;
using std::vector;

class ICriptoStock;
struct PathNode;


struct Symbol {
    string symbol;
    double comission;
    string base;
    string quote;
    double bestBidPrice; // здесь хранить уже логарифм по основанию 10 от цены
    double bestAskPrice; // здесь хранить уже логарифм по основанию 10 от цены
    double bestBidQty;
    double bestAskQty;

    vector<PathNode *> participates;
    BaseCriptoStock *criptostock;

    Symbol();
};
