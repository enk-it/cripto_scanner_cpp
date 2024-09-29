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
    string stockmarket_name;
    bool is_stopped=true;
public:
    BaseCriptoStock();  // Виртуальный деструктор
    explicit BaseCriptoStock(Scanner* scanner);  // Виртуальный деструктор
    ~BaseCriptoStock();  // Виртуальный деструктор
    void init(); // здесь делаем запрос насчёт доступных symbols (symbols), заполняем их, фильтруя по available_tokens. Подключаем websocket.
    string get_name();
    void trade ();
    void on_update (
        string symbol_name,
        double ask_price,
        double bid_price,
        double ask_qty,
        double bid_qty
        );

};


