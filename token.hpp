#ifndef TOKEN_HPP

#define TOKEN_HPP

#include <string>
#include <istream>

using std::string;

struct Token {
    string *type;
    string *name;
    static Token *from_cstr(const char*, const char*);
};

Token *parseline(std::istream &is);

#endif
