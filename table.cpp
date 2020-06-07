#include <vector>
#include <cassert>
#include <cstring>
#include "table.hpp"
#include "util.hpp"
#include <functional>
#include <ostream>

using std::vector;

vector<KeyValue> *parse_hyptable(std::istream& is)
{
    vector<KeyValue> *ret = new vector<KeyValue>();
    char line[LINE_LENGTH];
    char key[100], value[100];
    char c;
    Consume newline("\n");
    while (1) 
    {
        is >> c;
        if (c == '}')
        {
            is >> newline;
            break;
        }
        if (c != 'e')
        {
            std::cerr << "Syntax error, unexpected '" << c << "' at byte " << is.tellg()
                << std::endl;
            throw SyntaxError("");
        }
        is >> Consume("lement \"");
        is.getline(key, sizeof(key), '"');
        is >> Consume(" \"");
        is.getline(value, sizeof(value), '"');
        is >> Consume("\n");
        ret->push_back(KeyValue { key, value });
    }
    return ret;
}

Table::Table(vector<string> *headers, vector<string> *values) :
    m_headers(headers), m_values(values) 
{
    assert(values->size() % headers->size() == 0);
    m_size = values->size() / headers->size();
    for (int ii=0; ii < headers->size(); ii++)
    {
        m_header_map[(*headers)[ii]] = ii;
    }
}

int Table::size() const
{
    return m_size;
}

const vector<string>& Table::headers() const
{
    return *m_headers;
}

string& Table::at(int row, int col)
{
    return (*m_values)[col + row * m_headers->size()];
}

bool Table::operator==(const Table& rhs) const
{
    return *m_headers == *(rhs.m_headers) &&
        *m_values == *(rhs.m_values);
}

std::ostream& Table::operator<<(std::ostream& os) const
{
    os << "Table(" << std::endl;
    bool first = true;
    for (string& header : *m_headers)
    {
        if (!first)
            os << "\t";
        first = false;
        os << header;
    }
    os << std::endl;
}

unique_ptr<Table> Table::from_lines(std::istream& is)
{
    return unique_ptr<Table>();
}
