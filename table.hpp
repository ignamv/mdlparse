#ifndef TABLE_HPP

#define TABLE_HPP

#include <string>
#include <vector>
#include "util.hpp"

using std::string;
using std::vector;

struct KeyValue {
    string key;
    string value;
    bool operator==(const KeyValue& rhs) { 
        return key == rhs.key && value == rhs.value;
    }
};

inline bool operator==(const KeyValue& lhs, const KeyValue& rhs) {
    return lhs.key == rhs.key && lhs.value == lhs.value;
}

vector<KeyValue> *parse_hyptable(t_get_line get_line, void *userptr);

#endif
