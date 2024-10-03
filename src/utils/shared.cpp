//
// Created by silencer on 29.09.24.
//

#include "../../include/utils/shared.h"
#include "../../include/structure/PathNode.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.


struct PathNode;

std::string get_uuid() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return to_string(uuid);
}

double count_fr(int n, double comission_k) {
    // return log10(comission_k) * n;
    return pow(comission_k, n);
}


std::vector<PathNode*>* deep_copy(std::vector<PathNode *> *nodes) {
    std::vector<PathNode *>* result = new std::vector<PathNode *>({});

    for (int i = 0; i < nodes->size(); i++) {
        PathNode *node_copy = new PathNode();
        node_copy->symbol = nodes->at(i)->symbol;
        node_copy->is_reversed = nodes->at(i)->is_reversed;
        result->push_back(node_copy);
    }

    return result;
}