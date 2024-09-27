//
// Created by silencer on 17.09.24.
//
#pragma once

#include <string>
#include <vector>

struct Symbol;
using std::string;
using std::vector;

struct PathNode {
    Symbol* symbol;
    bool is_reversed;
    double* financial_result;
    PathNode();
    PathNode(Symbol* s, bool ir, double* fr) :
    symbol(s),
    is_reversed(ir),
    financial_result(fr)
    {}
};
