#pragma once

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
    virtual ~ICriptoStock();  // Виртуальный деструктор
    virtual void init(); // здесь делаем запрос насчёт доступных symbols (symbols), заполняем их, фильтруя по available_tokens. Подключаем websocket.
    virtual string get_name();
    virtual void set_symbols();
    virtual void trade ();
    virtual void on_update (
        string symbol_name,
        double ask_price,
        double bid_price,
        double ask_qty,
        double bid_qty
        );
};


