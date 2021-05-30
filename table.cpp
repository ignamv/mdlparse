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
    while (1) 
    {
        is >> c;
        if (c == '}')
        {
            is >> consume_newline;
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
        is >> consume_newline;
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

std::ostream& operator<<(std::ostream& os, const Table& table)
{
    os << "Table(" << std::endl;
    bool first = true;
    for (string& header : *(table.m_headers))
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
    char c;
    char header[100], value[100];
    int last_idx=-1;
    vector<string> *headers = new vector<string>();
    vector<string> *values = new vector<string>();
    vector<string>::const_iterator current_header;
    while (true)
    {
        is.get(c);
        if (c == '}')
        {
            is >> consume_newline;
            break;
        } else if(c == 'V') {
            is >> Consume("iew");
            is.ignore(999, '\n');
            continue;
        } 
        if (c != 'e')
        {
            is.clear();
            std::cout << "Unexpected '" << c << "' at byte " << is.tellg() << std::endl;
            throw SyntaxError("");
        }
        int idx;
        is >> Consume("lement ") >> idx >> Consume(" \"");
        is.getline(header, sizeof(header), '"');
        is >> Consume(" \"");
        is.getline(value, sizeof(value), '"');
        is >> consume_newline;
        values->push_back(value);
        if (idx == 0)
        {
            headers->push_back(header);
        } else {
            if(last_idx != idx) 
            {
                current_header = headers->cbegin();
            }
            if(*current_header != header)
            {
                std::cerr << "Expected header " << *current_header << " at byte " << is.tellg() << std::endl;
                throw SyntaxError("");
            }
            current_header++;
        }
        last_idx = idx;
    }
    return unique_ptr<Table>(new Table(headers, values));
}
