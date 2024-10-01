//
// Created by silencer on 29.09.24.
//

#include "../../include/utils/shared.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.


std::string get_uuid() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return to_string(uuid);
}

double count_fr(int n, double comission_k) {
    return log10(comission_k) * n;
}
