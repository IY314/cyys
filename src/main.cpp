#include <fstream>
#include <iostream>
#include <iterator>

#include "tokenizer.hpp"

int main(int argc, char *const *argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1]);

    if (!ifs)
    {
        std::cerr << "File not found: " << argv[1] << std::endl;
        return 1;
    }

    std::vector<Token> tokens = tokenize(argv[1],
                                         {std::istreambuf_iterator<char>(ifs),
                                          std::istreambuf_iterator<char>()});

    return tokens.empty();
}
