#ifndef TREE_HPP

#define TREE_HPP

#include <vector>
#include "token.hpp"
#include "util.hpp"

using std::vector;

struct Node {
    Token *token;
    vector<Node*> children;
};



vector<Node*> *parse_lines(t_get_line, void*);

#endif
