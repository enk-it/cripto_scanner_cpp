# Description:
This project was created in order to check if it is possible to make any profit from cross-exchange arbitrage.
For example, you can directly exchange your USDT on BTC on a binance, and my concept was that you probably can exchange USDT to something else (ETH for example) and then exchange it to BTC and that would be more profitable than direct exchange. So when you exchange your BTC back to USDT you get a little bit more than you had.
USDT -> ETH -> BTC -> USDT.

# Optimizations used:
1) Delta-updating

    After receiving the update, the influence of the old orderbook state is excluded from all Paths, taking into account the direction (Symbol or trading pair can be used for both sale and purchase), then the prices and quantities of Symbol are updated and its influence is included in all Paths again;

2) Hashmap-based Path generation

   To reduce the asymptotic complexity of Path's generation, each time a recursive function is called, a hash map is used instead of a vector with all possible Symbols. Hash map has a Symbol's symbol as a key and a vector of adjacent Symbols;

3) Log10(price) instead of price

   Addition/subtraction algorithms instead of division and multiplication in order to reduce multiplication/division error
# Details:
I accept that I am a market taker in every trade, because of need in instant exchange (exchange by market price is worse then placing your own order by the best price in terms of profit, but it allows you not to wait until someone buy/sell you something).
I accept that comission is 0.1% (you recieve 99.9% or 0.999 tokens after trade)  
Scanner has  implementation of 2 stocks (Binance, Bybit).


# Subjective opinion
Can you gain any profit of cross-exchange trading?  
Short answer: no.  
Long answer: probably yes, if you have zero-comission, zero exchange-delay, have some money in every token included in Path (to perform multiple trades, without needing to wait for last trade tokens to come), have some more implementation (more than just Binance and Bybit), but i suppose the the outcome will be just too small to cover the costs and efforts.



# Building:
```bash
git clone https://github.com/enk-it/cripto_scanner_cpp.git && cd cripto_scanner_cpp
mkdir build && cd build
cmake ../ && cmake --build .
```
I do not promise that this is memory safe, use it at your own risk.


# Thanks
Special thanks to [Steqa](https://github.com/steqa)  for implementing optimized path generator.
