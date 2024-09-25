#include "json.h"
#include <fstream>

using std::ifstream;


json parse_exchange(const string& path) {
    ifstream f(path);
    json data = json::parse(f);
    return data;
}
