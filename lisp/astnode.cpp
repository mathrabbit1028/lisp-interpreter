#include "astnode.hpp"

#include <iostream>
#include <typeinfo>

namespace lisp {

    /* LiteralNode */
    LiteralNode::LiteralNode(int value) {
        this->literal = value;
        this->type = "Literal";
    }
    LiteralNode::LiteralNode(char value) {
        this->literal = value;
        this->type = "Literal";
    }
    LiteralNode::LiteralNode(std::string value) {
        this->literal = value;
        this->type = "Literal";
    }
    LiteralNode::LiteralNode(bool value) {
        this->literal = value;
        this->type = "Literal";
    }
    LiteralNode::LiteralNode(std::nullptr_t value) {
        this->literal = value;
        this->type = "Literal";
    }

    void LiteralNode::print() const {
        std::visit([](const auto& val) {
            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::nullptr_t>) {
                std::cout << "[LiteralNode] nullptr\n";
            } else {
                std::cout << "[LiteralNode] " << val << " (" << typeid(val).name() << ")\n";
            }
        }, literal);
    }

    /* SymbolNode */
    SymbolNode::SymbolNode(std::string symbol) {
        this->symbol_name = symbol;
        this->type = "Symbol"; 
    }

    void SymbolNode::print() const {
        std::cout << "[SymbolNode] " << symbol_name << "\n";
    }

    /* ListNode */
    ListNode::ListNode(std::vector<ASTNode*> vec_nodes) {
        this->type = "List";
        this->sub_nodes = vec_nodes;
    }

    void ListNode::print() const {
        std::cout << "[ListNode] ";
        for (ASTNode* node_ptr : sub_nodes) {
            std::cout << node_ptr->type << " ";
        }
        std::cout << "(End)\n";
    }

    /* FunctionNode */
    FunctionNode::FunctionNode(std::vector<lisp::SymbolNode> parameters, std::vector<lisp::ASTNode*> codes) {
        this->type = "Function";
        this->parameters = std::unordered_map<std::string, ASTNode*>();
        for (auto symbol_node : parameters) {
            this->parameters.insert({symbol_node.symbol_name, new LiteralNode((int)0)});
        }
        this->codes = codes;
    }

    void FunctionNode::print() const {
        std::cout << "[FunctionNode] func( ";
        for (auto it = parameters.begin(); it != parameters.end(); it++)
            std::cout << it->first << " ";
        std::cout << ")\n";
    }

} // namespace lisp