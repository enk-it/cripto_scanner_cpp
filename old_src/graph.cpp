#include "graph.h"
#include <cmath>

using std::log10;


double count_fr(double comission_k,
                int n) {
    return log10(comission_k) * n;
}


void generate_paths(const string* start_token,
                    const string* current_token,
                    const vector<Symbol>* symbols_vec,
                    unordered_set<string>* history_set,
                    vector<string>* history,
                    vector<string>* history_token,
                    unordered_map<string, bool>* reverse,
                    vector<Path>* paths,
                    const int* maxlen)
{
    if (history_token->size() != 1 && *start_token == *current_token) {
        double financial_result = count_fr(0.999f, history->size());
        paths->emplace_back(
            financial_result,
            *history,
            *reverse
            );
        return;
    }
    if (history->size() >= *maxlen) {
        return;
    }

    for (const auto& symbol : *symbols_vec) {
        if (history_set->contains(symbol.symbol)) {
            continue;
        }
        if (symbol.base != *current_token && symbol.quote != *current_token) {
            continue;
        }
        string new_token;
        if (symbol.base == *current_token) {
            new_token = symbol.quote;
            (*reverse)[symbol.symbol] = false;
        } else {
            new_token = symbol.base;
            (*reverse)[symbol.symbol] = true;
        }

        if (new_token != *start_token && history_set->contains(new_token)) {
            reverse->erase(symbol.symbol);
            continue;
        }

        history->push_back(symbol.symbol);
        history_token->push_back(new_token);
        history_set->insert(symbol.symbol);

        // proceed

        generate_paths(
            start_token,
            &new_token,
            symbols_vec,
            history_set,
            history,
            history_token,
            reverse,
            paths,
            maxlen
            );

        history->pop_back();
        history_token->pop_back();
        history_set->erase(symbol.symbol);
        reverse->erase(symbol.symbol);
    }
}

vector<Path> get_paths(vector<Symbol> symbols_vec,
                       const int maxlen) {
    vector<Path> paths;

    unordered_set<string> history_set;
    vector<string> history;
    unordered_map<string, bool> reverse;
    vector<string> history_token {"USDT"};

    const string start_token = "USDT";

    generate_paths(
        &start_token,
        &start_token,
        &symbols_vec,
        &history_set,
        &history,
        &history_token,
        &reverse,
        &paths,
        &maxlen
    );

    return paths;
}