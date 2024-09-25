#include <iostream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <ixwebsocket/IXWebSocket.h>
#include "old_src/structures.h"
#include "old_src/graph.h"
#include "old_src/json.h"
#include "old_src/websocket.h"

using std::string;
using std::vector;
using std::ifstream;
using std::unordered_set;
using std::unordered_map;
using std::cout;
using std::endl;
using ix::WebSocket;
using nlohmann::json;


// Сделать общий класс описывающий биржу, от которого будут наследоваться классы с реализациями конкретных бирж.
// Сделать класс Symbol, instance которых будут храниться в биржах и будет обновляться информация для них, в частности price.
// Продумать механизм обнвления цены Symbol для обновления всех path. Использование rolling window и логарифмической арифметики.
// Также такой класс будет иметь ссылку на родителя-биржу, для того что бы можно было обращаться к ней с целью совершения транзакции
// Сделать класс Scanner который будет иметь список всех бирж, всех Symbol. Всех Paths.
// Учесть разные комиссии на разных биржах
// Каждая биржа должна иметь класс Wallet, который будет хранить текущие балансы по токенам
// Подумать над data-race проблемами и асинхронной структурой
// Класс Symbol должен иметь bids и asks

// перейти в pargma once

void update_symbol_price() {

}


int main() {
    unordered_map<string, Symbol> symbols_map; // словарь с парами
    vector<Symbol> symbols_vector;             // список пар
    json data = parse_exchange("/home/silencer/CLionProjects/cripto_scanner/data/exchangeInfo.json");
    const int maxlen = 6;


    const unordered_set<string> allowed_tokens = {
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

    for (int i = 0; i < data["symbols"].size(); ++i) {
        string symbol_name = data["symbols"][i]["symbol"].get<string>();
        string base = data["symbols"][i]["baseAsset"].get<string>();
        string quote = data["symbols"][i]["quoteAsset"].get<string>();
        string status = data["symbols"][i]["status"].get<string>();

        if (status != "TRADING") {
            continue;
        }
        if (!allowed_tokens.contains(base) || !allowed_tokens.contains(quote)) {
            continue;
        }

        vector<double*> participating;
        vector<bool> reverse;

        symbols_map[symbol_name] = Symbol(
            0, // = log10(1)
            symbol_name,
            base,
            quote,
            participating,
            reverse
        );
        symbols_vector.push_back(symbols_map[symbol_name]);
    }

    vector<Path> paths = get_paths(
    symbols_vector,
    maxlen
        );


    cout << "creating ws" << endl;
    WebSocket* webSocket = connect_websocket(&symbols_vector);
    cout << "created ws" << endl;


    return 0;
}
