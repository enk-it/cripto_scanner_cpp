//
// Created by silencer on 16.09.24.
//

#pragma once


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
    double financial_result; // здесь хранить логарифм
    vector<PathNode*> path;

    Path() = default;
    Path(
        double fr,
        vector<PathNode*> p
        ) : financial_result(fr), path(std::move(p)) {}
};


