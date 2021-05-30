#include <unity.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
#include "token.hpp"
#include "tree.hpp"
#include "table.hpp"


using std::vector;

void assert_tree_equals(Node *expected, Node *actual);

void setUp() {}
void tearDown() {}

typedef struct {
    const char *input;
    const char *expected_type;
    const char *expected_name;
} Testcase;

void assert_token_equals(Token *expected, Token *actual)
{
    TEST_ASSERT_EQUAL_STRING(expected->type->c_str(), actual->type->c_str());
    TEST_ASSERT_EQUAL_STRING(expected->name->c_str(), actual->name->c_str());
}

void assert_trees_equal(vector<Node*> *expected, vector<Node*> *actual)
{
    TEST_ASSERT_EQUAL_INT(expected->size(), actual->size());
    auto it_exp=expected->cbegin();
    auto it_act=actual->cbegin();
    for (; it_exp != expected->cend(); it_exp++, it_act++)
    {
        assert_tree_equals(*it_exp, *it_act);
    }
}

void assert_tree_equals(Node *expected, Node *actual)
{
    assert_token_equals(expected->token, actual->token);
    assert_trees_equal(&expected->children, &actual->children);
}

void test_case(Testcase *testcase) 
{
    std::stringstream is(testcase->input);
    Token *ret = parseline(is);
    Token *expected = Token::from_cstr(
            testcase->expected_type,
            testcase->expected_name);
    assert_token_equals(expected, ret);
}

void test_parseline() {
    vector<Testcase> testcases = {
        // No name
        {"data\n", "data", ""},
        {"caldata\n", "caldata", ""},
        {"OPTIMEDIT\n", "OPTIMEDIT", ""},
        {"circuitdeck\n", "circuitdeck", ""},
        {"dataset\n", "dataset", ""},
        // With name
        {"HYPTABLE \"Edit Sweep Info\"\n", "HYPTABLE", "Edit Sweep Info"},
        {"TABLE \"ICVIEWDATA\"\n", "TABLE", "ICVIEWDATA"},
        // LINK
        {"LINK DUT \"mydutname\"\n", "DUT", "mydutname"},
        {"LINK MODEL \"mymodelname\"\n", "MODEL", "mymodelname"},
    };
    for (auto testcase : testcases) {
        test_case(&testcase);
    }
}

typedef struct {
    vector<const char*>::const_iterator it;
    vector<const char*>::const_iterator end;
} t_userdata;

int get_line_from_vector(char *buffer, void *userptr)
{
    t_userdata &userdata = *((t_userdata*)userptr);
    auto &it = userdata.it;
    auto &end = userdata.end;
    if (it == end) {
        return 0;
    } else {
        strncpy(buffer, *it, 50); // TODO: real buffer size
        it++;
        return 1;
    }
}

void test_parselines(void)
{
    string input =
        "LINK MODEL \"mymodel\"\n"
        "{\n"
        "LINK DUT \"mydut\"\n"
        "{\n"
        "}\n"
        "}\n"
        "LINK MODEL \"mymodel2\"\n"
        "{\n"
        "}\n";
    std::stringstream is(input);
    vector<Node*> *ret = parse_lines(is);
    vector<Node*> expected = { 
        new Node {
            Token::from_cstr("MODEL", "mymodel"),
            vector<Node*> {
                new Node {Token::from_cstr("DUT", "mydut")},
            }},
        new Node {
            Token::from_cstr("MODEL", "mymodel2"),
            vector<Node*>()
            },
    };
    assert_trees_equal(&expected, ret);
}

void test_parse_hyptable()
{

    string input =
        "element \"key1\" \"value1\"\n"
        "element \"key 2\" \"value2\"\n"
        "element \"key3\" \"value 3\"\n"
        "}\n";
    std::stringstream is(input);
    vector<KeyValue> *ret = parse_hyptable(is);
    vector<KeyValue> expected = {
        {"key1", "value1"},
        {"key 2", "value2"},
        {"key3", "value 3"},
    };
    TEST_ASSERT_TRUE(*ret == expected);
}

void test_parse_dataset()
{

    vector<const char*> lines = {
        "datasize COMMON 3 1 1",
        "type COMMON",
        "point 0 1 1 1.89151e-12 0",
        "point 1 1 1 2.96206e-12 0",
    };
    t_userdata userdata = {lines.cbegin(), lines.cend()};
    /*vector<KeyValue> *ret = parse_dataset(get_line_from_vector, &userdata);
    vector<Dataset> expected = {
        {"key1", "value1"},
        {"key 2", "value2"},
        {"key3", "value 3"},
    };
    TEST_ASSERT_TRUE(*ret == expected);*/
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parseline);
    RUN_TEST(test_parselines);
    RUN_TEST(test_parse_hyptable);
    return UNITY_END();
}