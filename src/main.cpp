#include <iostream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <ixwebsocket/IXWebSocket.h>

#include "../include/class/Scanner.h"




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

    Scanner scanner = Scanner(allowed_tokens, 6);



    return 0;
}
