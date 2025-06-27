#include "lisp/lisp.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cerr << "There is not given file path.\n";
        return -1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << filename << "is inaccessible.\n";
        return -1;
    }

    std::string code;
    std::getline(file, code);

    lisp::Parser parser = lisp::read_str(code);

    parser.print();

    return 0;
}