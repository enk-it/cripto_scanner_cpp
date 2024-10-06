#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;
using std::vector;
using std::unordered_map;
using std::move;

struct PathNode;

struct Path {
    double financial_result; // здесь хранить логарифм
    vector<PathNode*> path;
    bool initialized = false;

    Path() = default;
    Path(
        const double fr,
        vector<PathNode*> p
        ) : financial_result(fr), path(p) {}
};


