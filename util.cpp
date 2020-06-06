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
