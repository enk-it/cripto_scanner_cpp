//
// Created by silencer on 16.09.24.
//

#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class ICriptoStock;
struct PathNode;


struct Symbol {
    string symbol;
    double comission;
    string base;
    string quote;
    double bestBidPrice;
    double bestAskPrice;
    double bestBidQty;
    double bestAskQty;

    vector<PathNode*> participates;
    ICriptoStock* criptostock;

    Symbol();
};



#endif //SYMBOL_H
