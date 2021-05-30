#ifndef TOKEN_HPP

#define TOKEN_HPP

#include <string>
#include <istream>
#include <memory>

using std::string;

class TokenData {
    public:
        virtual ~TokenData() {}
        friend std::ostream& operator<<(std::ostream& os, const TokenData&);
        friend bool operator==(const TokenData&, const TokenData&);
    private:
        virtual void to_stream(std::ostream& os) const = 0;
        virtual bool equals(const TokenData&) const = 0;
};

struct Token {
    string *type;
    string *name;
    std::unique_ptr<TokenData> data;
    static Token *from_cstr(const char*, const char*);
    friend std::ostream& operator<<(std::ostream& os, const Token&);
};

Token *parseline(std::istream &is);

#endif
