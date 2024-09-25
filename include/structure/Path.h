//
// Created by silencer on 16.09.24.
//

#ifndef PATH_H
#define PATH_H
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::unordered_map;
using std::move;

struct PathNode;

struct Path {
    double financial_result;
    vector<PathNode*> path;

    Path();
    Path(
        double fr,
        vector<PathNode*> p
        ) : financial_result(fr), path(move(p)) {}
};


#endif //PATH_H
