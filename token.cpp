#include <string>
#include <vector>
#include <cassert>
#include "token.hpp"

using std::string;
using std::vector;

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

