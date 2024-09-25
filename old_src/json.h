//
// Created by silencer on 08.09.24.
//

#ifndef JSON_H
#define JSON_H

#include <nlohmann/json.hpp>

using nlohmann::json;
using std::string;



json parse_exchange(const string& path);

#endif //JSON_H
