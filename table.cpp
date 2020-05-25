#include <vector>
#include <cassert>
#include <cstring>
#include "table.hpp"
#include "util.hpp"

using std::vector;

vector<KeyValue> *parse_hyptable(t_get_line get_line, void *userptr)
{
    vector<KeyValue> *ret = new vector<KeyValue>();
    char line[LINE_LENGTH];
    char key[100], value[100];
    while (1) 
    {
        assert(get_line(line, userptr));
        if (line[0] == '}')
            break;
        assert(startsWith(line, "element \""));
        char *keystart = &line[9];
        char *keyend = strchr(keystart, '"');
        int keylen = keyend - keystart;
        strncpy(key, keystart, keylen);
        key[keylen] = 0;
        assert(startsWith(keyend, "\" \""));
        char *valuestart = &keyend[4];
        char *valueend = strchr(valuestart, '"');
        int valuelen = valueend - valuestart;
        strncpy(value, valuestart, valueend-valuestart);
        value[valuelen] = 0;
        ret->push_back(KeyValue { key, value });
    }
    return ret;
}
