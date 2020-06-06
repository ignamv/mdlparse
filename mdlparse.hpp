#include <string>
#include <vector>
#include <utility>

using std::string;
using std::vector;
using std::pair;




enum NodeDataType { keyvalues, text, dataset };

struct NodeData {
    NodeDataType type;
    union {
        vector<KeyValue> *keyvalues;
        string text;
        Dataset *dataset;
    };
};

