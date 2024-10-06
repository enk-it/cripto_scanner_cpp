#include "../../include/structure/Symbol.h"


Symbol::Symbol() {
    symbol = "";
    comission = 0.999;
    base = "";
    quote = "";
    bestBidPrice = 0.0;
    bestAskPrice = 0.0;
    bestBidQty = 0.0;
    bestAskQty = 0.0;
    criptostock = nullptr;
}
