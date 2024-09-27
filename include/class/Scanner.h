//
// Created by silencer on 17.09.24.
//

#ifndef SCANNER_H
#define SCANNER_H
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
    unordered_map<string, Symbol*> symbols;
    vector<Path> paths;
    int maxlen;

public:
    Scanner();
    ~Scanner();
    Scanner(vector<string> t, int m);

    void updateSymbol(string ticker, double best_ask_qty, double best_ask_price , double best_bid_qty, double best_bid_price);

    void addSymbol(Symbol* new_symbol);

    void scanForBestFinancialResult();

};



#endif //SCANNER_H
