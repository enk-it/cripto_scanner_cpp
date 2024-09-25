//
// Created by silencer on 17.09.24.
//

#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include <unordered_map>


using std::unordered_map;
using std::string;
using std::vector;

class ICriptoStock;

class Wallet {
protected:
    ICriptoStock* criptostock;
    unordered_map<string, double> balance;
public:
    Wallet();
    ~Wallet();
    Wallet(ICriptoStock* cs) : criptostock(cs) {};

    void setDefaultBalance();
    void setBalanceFromCache();

};



#endif //WALLET_H
