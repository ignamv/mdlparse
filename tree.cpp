#include <vector>
#include "tree.hpp"
#include "token.hpp"

using std::vector;
using std::swap;

#define LINE_LENGTH 200

vector<Node*> *parse_lines(t_get_line get_line, void *userptr)
{
    vector<Node*> *ret = new vector<Node*>();
    vector<Node*> path;
    char buffer1[LINE_LENGTH], buffer2[LINE_LENGTH];
    char *line = buffer1, *previous_line = buffer2;
    while (get_line(line, userptr)) {
        if (line[0] == '{') {
            Token *tok = parseline(previous_line);
            Node *node = new Node {tok, {}};
            if (!path.empty()) {
                path.back()->children.push_back(node);
            }
            path.push_back(node);
        } else if(line[0] == '}') {
            Node *node = path.back();
            path.pop_back();
            if (path.empty()) {
                // Found a root node
                ret->push_back(node);
            }
        }
        swap(line, previous_line);
    }
    return ret;
}
