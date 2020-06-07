#ifndef DATASET_FRIEND_HPP

#define DATASET_FRIEND_HPP

#include <tuple>

using std::tie;

struct Datasize {
    Dataset::Type type;
    int size;
    int ports1;
    int ports2;
    inline bool operator==(const Datasize &rhs) {
        return tie(size, ports1, ports2) == 
            tie(rhs.size, rhs.ports1, rhs.ports2);
    }
};

inline bool operator==(const Datasize& lhs, const Datasize &rhs) {
    return tie(lhs.size, lhs.ports1, lhs.ports2) == 
        tie(rhs.size, rhs.ports1, rhs.ports2);
}

Datasize parse_datasize(istream &is);
Dataset::Type parse_type(istream &is);

#endif
