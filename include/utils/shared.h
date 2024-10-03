//
// Created by silencer on 29.09.24.
//

#ifndef SHARED_H
#define SHARED_H
#include <string>
#include <vector>


struct PathNode;

std::string get_uuid();

double count_fr(int n, double comission_k = 0.999);

std::vector<PathNode *>* deep_copy(std::vector<PathNode *> *nodes);

#endif //SHARED_H
