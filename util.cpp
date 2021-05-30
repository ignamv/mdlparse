#include <cstring>
#include <iostream>
#include "util.hpp"

bool startsWith(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix)) == 0;
}


LineVectorReader::LineVectorReader(const vector<string>& lines) : 
    m_lines(lines), m_it(m_lines.cbegin()) { }

int LineVectorReader::operator()(char *buffer) {
    if (m_it == m_lines.cend()) {
        return 0;
    } else {
        strncpy(buffer, m_it->c_str(), LINE_LENGTH);
        m_it++;
        return 1;
    }
}

std::istream& consume_newline(std::istream& is)
{
    char c;
    is.get(c);
    if (is.eof())
    {
        std::cerr << "At byte " << is.tellg() << std::endl;
        throw SyntaxError("Unexpected EOF");
    }
    if (c == '\n')
    {
        return is;
    } else if(c == '\r') {
        is.get(c);
        if (is.eof())
        {
            throw SyntaxError("Unexpected EOF");
        }
        if (c == '\n') {
            return is;
        }
    }
    std::cerr << "At byte " << is.tellg() << std::endl;
    throw SyntaxError("Expected newline");
}

std::istream& operator>>(std::istream& is, const Consume& consume)
{
    char actual;
    for (const char expected : consume.m_str)
    {
        is.get(actual);
        if (actual != expected)
        {
            std::cerr << "Expected " << consume.m_str << ", got " << actual << 
                " (" << (int)actual << ") at byte " << is.tellg() << std::endl;
            throw std::logic_error(consume.m_str);
        }
    }
    return is;
}
