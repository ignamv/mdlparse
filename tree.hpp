#ifndef TREE_HPP

#define TREE_HPP

#include <vector>
#include <istream>
#include "token.hpp"
#include "util.hpp"

using std::vector;

struct Node {
    Token *token;
    vector<Node*> children;
};



vector<Node*> *parse_lines(std::istream&);

#endif
