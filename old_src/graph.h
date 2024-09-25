#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>  // Включаем минимально необходимый заголовок
#include "../include/structures.h"

using std::unordered_set;
using std::unordered_map;
using std::string;
using std::vector;


double count_fr(double comission_k,
                int n);

void generate_paths(const string* start_token,
                    const string* current_token,
                    const vector<Symbol>* symbols_vec,
                    unordered_set<string>* history_set,
                    vector<string>* history,
                    vector<string>* history_token,
                    unordered_map<string, bool>* reverse,
                    vector<Path>* paths,
                    const int* maxlen);

vector<Path> get_paths(vector<Symbol> symbols_vec,
                       int maxlen);


#endif //GRAPH_H
