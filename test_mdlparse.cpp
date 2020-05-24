#include <unity.h>
#include <vector>
#include "mdlparse.hpp"
#include <cstring>

#include <iostream>

using std::vector;

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
    Token *ret = parseline(testcase->input);
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
    vector<const char*>::const_iterator &it;
    vector<const char*>::const_iterator &end;
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
    vector<const char*> lines = {
        "LINK MODEL \"mymodel\"",
        "{",
        "LINK DUT \"mydut\"",
        "{",
        "}",
        "}",
        "LINK MODEL \"mymodel2\"",
        "{",
        "}",
    };
    auto it = lines.cbegin();
    auto end = lines.cend();
    t_userdata userdata = {it, end};
    vector<Node*> *ret = parse_lines(get_line_from_vector, &userdata);
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

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parseline);
    RUN_TEST(test_parselines);
    return UNITY_END();
}
