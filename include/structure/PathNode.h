#pragma once

#include <string>
#include <vector>

struct Symbol;
struct Path;
using std::string;
using std::vector;

struct PathNode {
    Symbol* symbol;
    bool is_reversed;
    Path* path;
    PathNode() = default;
    PathNode(Symbol* s, bool ir) :
    symbol(s),
    is_reversed(ir),
    path(nullptr)
    {}
};
