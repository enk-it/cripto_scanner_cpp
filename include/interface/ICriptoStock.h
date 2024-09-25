#ifndef ICRIPTOSTOCK_H
#define ICRIPTOSTOCK_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::unordered_map;
using std::unordered_map;
using std::vector;
using std::string;
using std::move;

struct Symbol;
class Scanner;

class ICriptoStock {
protected:
    Scanner* scanner;
    string stockmarket_name;
    vector<Symbol> symbols;
    bool is_stopped=true;
public:
    explicit ICriptoStock();
    explicit ICriptoStock(Scanner* s, string n) :
    scanner(s),
    stockmarket_name(move(n))
    {}
    virtual ~ICriptoStock() = default;  // Виртуальный деструктор

    virtual void init(); // здесь делаем запрос насчёт доступных symbols (symbols), заполняем их, фильтруя по available_tokens. Подключаем websocket.

    virtual string get_name();
};


#endif //ICRIPTOSTOCK_H
