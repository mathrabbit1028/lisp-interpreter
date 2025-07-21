#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "astnode.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

namespace lisp {
    class Environment {
    public:
        std::unordered_map<std::string, ASTNode*> symbols;
        Environment(std::vector<SymbolNode>, std::vector<ASTNode*>);
        Environment();
        void add(SymbolNode name, ASTNode* node);
        ASTNode* get(std::string key);
        void print();
    };
} // namespace lisp

#endif