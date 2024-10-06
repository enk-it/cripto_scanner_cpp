#include <iostream>
#include <unordered_set>
#include <nlohmann/json.hpp>

#include "../include/class/Scanner.h"
#include "../include/class/BinanceImpl.h"
#include "../include/class/BybitImpl.h"


using std::string;
using std::vector;
using std::ifstream;
using std::unordered_set;
using std::unordered_map;
using std::cout;
using std::endl;
using nlohmann::json;


void update_symbol_price() {
}


int main() {
    std::cout << "Первая строчка" << std::endl;
    const int maxlen = 6;
    vector<string> allowed_tokens = {
        "BRL",
        "USDT",
        "BTC",
        "BNB",
        "ETH",
        "TRY",
        "DOGE",
        "SHIB",
        "XRP",
        "ADA",
        "DAI",
        "DOT",
        "LINK",
        "MATIC",
        "SOL",
        "TRX",
        "AAVE",
        "FTM",
        "AVAX",
        "ATOM",
        "EUR",
    };


    net::io_context ioc;

    Scanner *scanner = new Scanner(allowed_tokens, maxlen, &ioc);

    BinanceImpl *binance = new BinanceImpl(scanner, "i");
    BybitImpl *bybit = new BybitImpl(scanner, "y");

    scanner->add_stock(binance);
    scanner->add_stock(bybit);

    scanner->init();

    return 0;
}
