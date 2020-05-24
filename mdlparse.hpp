#include <string>
#include <vector>
#include <utility>

using std::string;
using std::vector;
using std::pair;

struct Token {
    string *type;
    string *name;
    static Token *from_cstr(const char*, const char*);
};

enum NodeDataType { keyvalues, text, dataset };

class Dataset {
    enum DatasetType { measured, simulated, common, both };
    public:
        Dataset(DatasetType, float*, float*);
};

struct KeyValue {
    const char *key;
    const char *value;
};

struct NodeData {
    NodeDataType type;
    union {
        vector<KeyValue> *keyvalues;
        string text;
        Dataset *dataset;
    };
};

struct Node {
    Token *token;
    vector<Node*> children;
};

Token *parseline(const char *line);

typedef int (*t_get_line)(char*, void*);

vector<Node*> *parse_lines(t_get_line, void*);

void assert_trees_equal(vector<Node*>, vector<Node*> *);
void assert_tree_equals(Node *, Node *);
