#ifndef UTIL_HPP

#define UTIL_HPP

#include <stdexcept>
#include <iostream>
#include <vector>

using std::vector;
using std::string;

#define LINE_LENGTH 200

bool startsWith(const char *str, const char *prefix);

typedef int (*t_get_line)(char*, void*);

class SyntaxError : public std::logic_error {
    public:
        SyntaxError(const std::string& what_arg): std::logic_error(what_arg) {}
};

#define ASSERT_EQUAL_PRINTABLE(expected,actual) assert_equal_printable((expected),(actual),__FILE__,__LINE__)
template<class T1, class T2>
void assert_equal_printable(const T1& expected, const T2& actual, const char *file, int line)
{
    if (expected == actual)
        return;
    std::cout << "FAIL at " << file << ":" << line << std::endl;
    std::cout << "Expected:" << std::endl << expected << std::endl;
    std::cout << "Actual:" << std::endl << actual << std::endl;
    throw;
}

class LineVectorReader
{
    public:
        LineVectorReader(const vector<string>& lines);
        int operator()(char *buffer);
    private:
        const vector<string>& m_lines;
        vector<string>::const_iterator m_it;
};

#endif
