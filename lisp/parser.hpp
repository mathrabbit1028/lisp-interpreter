#ifndef PARSER_HPP
#define PARSER_HPP

#include <cassert>
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <queue>
#include <variant>
#include <vector>

namespace lisp {
    std::vector<std::string> tokenize(std::string str);

    class ASTNode {
    public:
        std::string type;
        virtual ~ASTNode();
        virtual void print() const;
    };

    typedef std::variant<int, char, std::string, bool, std::nullptr_t> Literal;
    class LiteralNode : public ASTNode {
    private:
        std::string type;
        Literal literal;
    public:
        LiteralNode(int value);
        void print() const override;
    };

    class SymbolNode : public ASTNode {
    private:
        std::string type;
        std::string symbol_name;
    public:
        SymbolNode(std::string symbol);
        void print() const override;
    };

    class ListNode : public ASTNode {
    private:
        std::string type;
        std::vector<ASTNode*> sub_nodes;
    public:
        ListNode(std::vector<ASTNode*> vec_nodes);
        void print() const override;
    };
};


#endif