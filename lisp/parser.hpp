#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <variant>
#include <queue>
#include <exception>

namespace lisp {

    class ASTNode {
    public:
        std::string type;
        std::vector<ASTNode*> sub_nodes;
        virtual ~ASTNode() = default;
        virtual void print() const = 0;
    };

    typedef std::variant<int, char, std::string, bool, std::nullptr_t> Literal;

    class syntexError : public std::exception {
    private:
        char* specific_reason;
    public:
        syntexError(char* spec);
        const char* what() const noexcept override;
    };

    class LiteralNode : public ASTNode {
    private:
        Literal literal;
    public:
        LiteralNode(int value);
        LiteralNode(char value);
        LiteralNode(std::string value);
        LiteralNode(bool value);
        LiteralNode(std::nullptr_t value);
        void print() const override;
    };

    class SymbolNode : public ASTNode {
    private:
        std::string symbol_name;
    public:
        SymbolNode(std::string symbol);
        void print() const override;
    };

    class ListNode : public ASTNode {
    public:
        ListNode(std::vector<ASTNode*> vec_nodes);
        void print() const override;
    };

    class Parser {
    private:
        ASTNode* root;

        enum class NodeType { Literal, Symbol };
        enum class LiteralType { Int, Char, String, Bool, Null };

        NodeType node_type_finder(std::string str);
        LiteralType literal_type_finder(std::string str);
        int text_to_int(std::string text);
        char text_to_char(std::string text);
        std::string text_to_string(std::string text);
        bool text_to_bool(std::string text);
        std::nullptr_t text_to_null(std::string text);
        ListNode* parse_list(std::queue<std::string>& token_queue);
        void print_node(ASTNode* node, int depth);

    public:
        Parser(std::vector<std::string> token_list);
        void print();
    };

    std::vector<std::string> tokenize(std::string str);
    Parser read_str(std::string str);

} // namespace lisp

#endif