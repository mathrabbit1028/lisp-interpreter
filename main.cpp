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
    lisp::Evaluator evaluator = lisp::Evaluator();
    while (std::getline(file, code)) {
        if (code.length() == 0) continue;

        lisp::Parser parser = lisp::read_str(code);

        parser.print();

        lisp::Literal result = evaluator.run(((lisp::ListNode*)parser.root)->sub_nodes[0]);

        std::visit([](const auto& val) {
            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::nullptr_t>) {
                std::cout << "nullptr\n";
            } else {
                std::cout << val << " (" << typeid(val).name() << ")\n";
            }
        }, result);
    }

    return 0;
}