#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

namespace lisp {

    class ASTNode {
    public:
        std::string type;
        virtual ~ASTNode() = default;
        virtual void print() const = 0;
    };

    typedef std::variant<int, char, std::string, bool, std::nullptr_t> Literal;

    class LiteralNode : public ASTNode {
    private:
    public:
        Literal literal;
        LiteralNode(int value);
        LiteralNode(char value);
        LiteralNode(std::string value);
        LiteralNode(bool value);
        LiteralNode(std::nullptr_t value);
        void print() const override;
    };

    class SymbolNode : public ASTNode {
    public:
        std::string symbol_name;
        SymbolNode(std::string symbol);
        void print() const override;
    };

    class ListNode : public ASTNode {
    public:
        std::vector<ASTNode*> sub_nodes;
        ListNode(std::vector<ASTNode*> vec_nodes);
        void print() const override;
    };

    class FunctionNode : public ASTNode {
    private:
    public:
        std::unordered_map<std::string, ASTNode*> parameters;
        std::vector<ASTNode*> codes;
        FunctionNode(std::vector<lisp::SymbolNode> parameters, std::vector<lisp::ASTNode*> codes);
        void print() const override;
    };
    
} // namespace lisp

#endif