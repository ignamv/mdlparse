#include <vector>
#include <memory>
#include "tree.hpp"
#include "token.hpp"
#include "dataset.hpp"

using std::vector;
using std::swap;

#define LINE_LENGTH 200

vector<Node*> *parse_lines(std::istream &is)
{
    vector<Node*> *ret = new vector<Node*>();
    vector<Node*> path;
    int current_loc, previous_loc;
    char c;
    Consume newline("\n");
    while (true)
    {
        current_loc = is.tellg();
        is.get(c);
        if (is.eof())
        {
            if (!path.empty())
            {
                std::cout << "Unclosed tokens:" << std::endl;
                for (auto it : path)
                {
                    const Token& tok = *((*it).token);
                    std::cout << *(tok.type) << " : " << *(tok.name) 
                        << std::endl;
                }
                throw SyntaxError("Unclosed tokens");
            }
            break;
        }
        if (c == '{') {
            is >> consume_newline;
            is.seekg(previous_loc);
            Token *tok = parseline(is);
            is.seekg(current_loc+1);
            is >> consume_newline;
            if (*(tok->type) == "dataset")
            {
                unique_ptr<Dataset> ds = Dataset::from_lines(is);
                tok->data = std::move(ds);
                is.seekg(-2, std::ios_base::cur);
                if (is.peek() != '{')
                {
                    is.seekg(-1, std::ios_base::cur);
                }
            }
            Node *node = new Node {tok, {}};
            if (!path.empty()) {
                path.back()->children.push_back(node);
            }
            path.push_back(node);
        } else if(c == '}') {
            is >> newline;
            if (path.empty())
                throw SyntaxError("Unexpected }");
            Node *node = path.back();
            path.pop_back();
            if (path.empty()) {
                // Found a root node
                ret->push_back(node);
            }
        } else {
            char line[200];
            is.getline(line, sizeof(line));
            std::cout << "Ignoring " << c << line << std::endl;
            //is.ignore('\n');
        }
        swap(current_loc, previous_loc);
    }
    return ret;
}

void Node::to_stream(ostream& os, int indent) const
{
    os << token << std::endl;
    for (auto child : children)
    {
        child->to_stream(os, indent+1);
    }
}

ostream& operator<<(ostream& os, const Node& node)
{
    node.to_stream(os, 0);
    return os;
}
