#ifndef PARSER_HPP
#define PARSER_HPP

#include "astnode.hpp"
#include "error.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <string>

namespace lisp {

    class Parser {
    private:
        enum class NodeType { Literal, Symbol, Function };
        enum class LiteralType { Int, Char, String, Bool, Null };

        NodeType node_type_finder(std::string str);
        LiteralType literal_type_finder(std::string str);
        int text_to_int(std::string text);
        char text_to_char(std::string text);
        std::string text_to_string(std::string text);
        bool text_to_bool(std::string text);
        std::nullptr_t text_to_null(std::string text);
        ASTNode* token_to_node(std::string token);
        ListNode* parse_list(std::queue<std::string>& token_queue);
        void print_node(ASTNode* node, int depth);

    public:
        ASTNode* root = new ListNode({});
        Parser(std::vector<std::string> token_list);
        void print();
    };

    std::vector<std::string> tokenize(std::string str);
    Parser read_str(std::string str);

} // namespace lisp

#endif