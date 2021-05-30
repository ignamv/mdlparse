#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include "unity.h"
#include "util.hpp"
#include "dataset.hpp"
#include "dataset_friend.hpp"

using std::vector;
using std::string;

void setUp() {}
void tearDown() {}

void test_parse_type()
{
    struct TestCase {
        string input;
        Dataset::Type expected;
    };
    vector<TestCase> testcases = vector<TestCase> {
        {"MEAS", Dataset::Type::t_measured},
        {"SIMU", Dataset::Type::t_simulated},
        {"COMMON", Dataset::Type::t_common},
        {"BOTH", Dataset::Type::t_both},
    };
    for (auto [input, expected] : testcases)
    {
        const char *readptr;
        std::stringstream is(input);
        Dataset::Type actual = parse_type(is);
        TEST_ASSERT_EQUAL_INT(expected, actual);
        //TEST_ASSERT_EQUAL_INT(input.size(), readptr-input.c_str());
    }
}

void test_parse_datasize()
{
    struct TestCase {
        string input;
        Datasize expected;
    };
    vector<TestCase> testcases = vector<TestCase> {
        {"datasize MEAS 1 2 3\n", {Dataset::Type::t_measured, 1, 2, 3}},
        {"datasize SIMU 4 5 6\n", {Dataset::Type::t_simulated, 4, 5, 6}},
        {"datasize COMMON 7 8 9\n", {Dataset::Type::t_common, 7, 8, 9}},
        {"datasize BOTH 6 6 6\n", {Dataset::Type::t_both, 6, 6, 6}},
    };
    for (auto [input, expected] : testcases)
    {
        std::stringstream is(input);
        auto actual = parse_datasize(is);
        TEST_ASSERT_TRUE(expected == actual);
    }
}

void test_parse_dataset()
{
    struct TestCase {
        string lines;
        Dataset expected;
    };
    TestCase mytestcase1 =
        {
            "datasize MEAS 2 1 1\n"
            "type MEAS\r\n"
            "point 0 1 1 1.0E-01 2.0E-2\n"
            "point 1 1 1 3.0E-03 4.0E-4\n"
            "}\n",
         Dataset(Dataset::Type::t_measured,
                 std::unique_ptr<vector<double>>(
                     new vector<double>{1e-1,2e-2,3e-3,4e-4}), 
                 std::unique_ptr<vector<double>>())};
    TestCase mytestcase2 =
        {
            "datasize BOTH 2 1 1\n"
            "type SIMU\n"
            "point 0 1 1 4.0E-01 3.0E-2\n"
            "point 1 1 1 2.0E-03 1.0E-4\n"
            "type MEAS\n"
            "point 0 1 1 1.0E-01 2.0E-2\n"
            "point 1 1 1 3.0E-03 4.0E-4\n"
            "}\n",
         Dataset(Dataset::Type::t_both,
                 std::unique_ptr<vector<double>>(
                     new vector<double>{1e-1,2e-2,3e-3,4e-4}), 
                 std::unique_ptr<vector<double>>(
                     new vector<double>{4e-1,3e-2,2e-3,1e-4})
                 )};
    TestCase mytestcase3 =
        {
            "datasize SIMU 2 1 1\n"
            "type SIMU\r\n"
            "point 0 1 1 1.0E-01 2.0E-2\r\n"
            "point 1 1 1 3.0E-03 4.0E-4\n"
            "}\n",
         Dataset(Dataset::Type::t_simulated,
                 std::unique_ptr<vector<double>>(
                     new vector<double>{1e-1,2e-2,3e-3,4e-4}), 
                 std::unique_ptr<vector<double>>())};
    vector<TestCase*> testcases = {&mytestcase1, &mytestcase2, &mytestcase3};
    for (TestCase *testcase : testcases)
    {
        std::stringstream ss(testcase->lines);
        auto actual = Dataset::from_lines(ss);
        ASSERT_EQUAL_PRINTABLE(testcase->expected, *actual);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parse_type);
    RUN_TEST(test_parse_datasize);
    RUN_TEST(test_parse_dataset);
    return UNITY_END();
}
