#include "tree.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename]" << std::endl;
        return -1;
    }
    std::fstream fd(argv[1]);
    parse_lines(fd);
    return 0;
}
