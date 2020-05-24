#include <string>
#include <vector>
#include <cctype>
#include <cassert>
#include <utility>

#include "mdlparse.hpp"

using std::string;
using std::vector;
using std::swap;

Token *Token::from_cstr(const char *type, const char *name)
{
    return new Token { new string(type), new string(name) };
}

// Parse a line into a Token
Token *parseline(const char *line) {
    Token *ret = new Token {new string(""), new string("")};
    const char *read = line;
    while (!isspace(*read)) {
        ret->type->push_back(*read++);
    }
    if (
            ret->type->compare("caldata") == 0 || 
            ret->type->compare("OPTIMEDIT") == 0|| 
            ret->type->compare("circuitdeck") == 0 ||
            ret->type->compare("data") == 0 ||
            ret->type->compare("dataset") == 0)
    {
        // No name
        return ret;
    }
    if (ret->type->compare("LINK") == 0) {
        // One more word for the type of LINK
        read++;
        ret->type->clear();
        while (!isspace(*read)) {
            ret->type->push_back(*read++);
        }
    }
    read++;
    assert(*read++ == '"');
    while (*read != '"') {
        ret->name->push_back(*read++);
    }
    return ret;
}

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
