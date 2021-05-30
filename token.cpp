#include <string>
#include <vector>
#include <cstring>
#include "token.hpp"
#include "util.hpp"

using std::string;
using std::vector;

Token *Token::from_cstr(const char *type, const char *name)
{
    return new Token { new string(type), new string(name) };
}

void readword(std::istream& is, char *buffer, int length)
{
    char c;
    while(length--)
    {
        is.get(c);
        if (isspace(c))
        {
            *buffer++ = 0;
            break;
        }
        *buffer++ = c;
    }
}

// Parse a line into a Token
Token *parseline(std::istream &is)
{
    char type[150], name[150];
    readword(is, type, sizeof(type));
    if (
            !strcmp(type, "caldata") ||
            !strcmp(type, "OPTIMEDIT") ||
            !strcmp(type, "circuitdeck") ||
            !strcmp(type, "data") ||
            !strcmp(type, "dataset") )
    {
        // No name
        return new Token {new string(type), new string("")};
    }
    if (!strcmp(type, "LINK"))
    {
        // Disregard 'LINK', read another word for the type of LINK
        readword(is, type, sizeof(type));
    }
    is >> Consume("\"");
    is.getline(name, sizeof(name), '"');
    is.ignore('\n');
    return new Token{new string(type), new string(name)};
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    os << *(token.type) << " : " << *(token.name); 
    return os;
}

std::ostream& operator<<(std::ostream& os, const TokenData& tokendata)
{
    tokendata.to_stream(os);
    return os;
}

bool operator==(const TokenData& l, const TokenData& r)
{
    if (typeid(l) != typeid(r))
        return false;
    return l.equals(r);
}
