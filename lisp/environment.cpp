#include "environment.hpp"

namespace lisp {
    Environment::Environment(std::vector<SymbolNode> names, std::vector<ASTNode*> ASTnodes) {
        for (int i = 0; i < names.size(); i++)
            this->add(names[i], ASTnodes[i]);
    }

    Environment::Environment() {
        return;
    }

    void Environment::add(SymbolNode name, ASTNode* node) {
        // check
        this->symbols.insert({name.symbol_name, node});
    }

    ASTNode* Environment::get(std::string key) {
        if (this->symbols.find(key) == this->symbols.end()) return nullptr;
        return this->symbols[key];
    }

    void Environment::print() {
        std::cout << "{Environment}\n";
        for (auto it = this->symbols.begin(); it != this->symbols.end(); it++) {
            std::cout << "\t\t" << it->first << " ";
            it->second->print();
        }
    }
}