//
// Created by silencer on 19.09.24.
//

#include "../../include/class/BaseCriptoStock.h"


BaseCriptoStock::BaseCriptoStock() {
    this->scanner = nullptr;
};
BaseCriptoStock::BaseCriptoStock(Scanner* scanner) {
    this->scanner = scanner;
};

BaseCriptoStock::~BaseCriptoStock() = default;


void BaseCriptoStock::init () {

}

string BaseCriptoStock::get_name () {
    return this->stockmarket_name;
}

void BaseCriptoStock::trade () {

}

void BaseCriptoStock::on_update (
    string symbol_name,
    double ask_price,
    double bid_price,
    double ask_qty,
    double bid_qty
    ) {

}