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


void BaseCriptoStock::trade () {

}

void BaseCriptoStock::allow_update () {
    this->to_update = true;
}

void BaseCriptoStock::on_update (
    const string& symbol_name,
    double ask_qty,
    double ask_price,
    double bid_qty,
    double bid_price
    ) {
    this->scanner->update_symbol(
                this->stockmarket_name + symbol_name,
                ask_qty,
                ask_price,
                bid_qty,
                bid_price
            );
}