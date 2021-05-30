#ifndef TREE_HPP

#define TREE_HPP

#include <vector>
#include <istream>
#include <ostream>
#include "token.hpp"
#include "util.hpp"

using std::vector;

struct Node {
    Token *token;
    vector<Node*> children;
    friend std::ostream& operator<<(std::ostream& os, const Node&);
    void to_stream(std::ostream& os, int indent) const;
};



vector<Node*> *parse_lines(std::istream&);

#endif
