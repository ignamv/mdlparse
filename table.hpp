#ifndef TABLE_HPP

#define TABLE_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <istream>
#include "util.hpp"

using std::string;
using std::unique_ptr;
using std::vector;
using std::map;

struct KeyValue {
    string key;
    string value;
    bool operator==(const KeyValue& rhs) const { 
        return key == rhs.key && value == rhs.value;
    }
};

vector<KeyValue> *parse_hyptable(std::istream& is);

class Table 
{
    public:
        Table(vector<string> *headers,
                vector<string> *values);
        int size() const;
        const vector<string> &headers() const;
        string& at(int, int);
        const string& cat(int, int) const;
        bool operator==(const Table& rhs) const;
        std::ostream& operator<<(std::ostream&) const;
        static unique_ptr<Table> from_lines(std::istream&);
    private:
        unique_ptr<vector<string>> m_values;
        unique_ptr<vector<string>> m_headers;
        map<string,int> m_header_map;
        int m_size;
};

#endif
