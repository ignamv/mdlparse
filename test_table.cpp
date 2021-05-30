#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <sstream>
#include "unity.h"
#include "table.hpp"

using std::vector;
using std::map;
using std::string;

void setUp() {}
void tearDown() {}

#include <ostream>
std::ostream& operator<<(std::ostream &os, const vector<string>& values)
{
    return os;
}

void test_table_class()
{
    auto headers = new vector<string>{"name", "value"};
    Table tab = Table(headers,
            new vector<string>{"k1","v1","k2","v2"});
    TEST_ASSERT_EQUAL_INT(2, tab.size());
    ASSERT_EQUAL_PRINTABLE(*headers, tab.headers());
    ASSERT_EQUAL_PRINTABLE("k1", tab.at(0,0));
    ASSERT_EQUAL_PRINTABLE("v1", tab.at(0,1));
    ASSERT_EQUAL_PRINTABLE("k2", tab.at(1,0));
    ASSERT_EQUAL_PRINTABLE("v2", tab.at(1,1));
}

void test_parse_table()
{
    struct TestCase {
        string lines;
        Table expected;
    };
    TestCase mytestcase1 =
        {
            "element 0 \"Name\" \"k1\"\n"
            "element 0 \"Value\" \"v1\"\n"
            "element 1 \"Name\" \"k2\"\r\n"
            "element 1 \"Value\" \"v2\"\n"
         , Table(new vector<string>{"Name","Value"},
                 new vector<string>{"k1","v1","k2","v2"})};
    vector<TestCase*> testcases = {&mytestcase1};
    for (TestCase *testcase : testcases)
    {
        std::stringstream is(testcase->lines);
        auto actual = Table::from_lines(is);
        //ASSERT_EQUAL_PRINTABLE(testcase->expected, *actual);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_table_class);
    return UNITY_END();
}

