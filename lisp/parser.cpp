#include "parser.hpp"
#include <iostream>
#include <cassert>

namespace lisp {

    syntexError::syntexError(char* spec) : specific_reason(spec) {}

    const char* syntexError::what() const noexcept {
        return specific_reason;
    }

    LiteralNode::LiteralNode(int value) : literal(value) { type = "Literal"; }
    LiteralNode::LiteralNode(char value) : literal(value) { type = "Literal"; }
    LiteralNode::LiteralNode(std::string value) : literal(value) { type = "Literal"; }
    LiteralNode::LiteralNode(bool value) : literal(value) { type = "Literal"; }
    LiteralNode::LiteralNode(std::nullptr_t value) : literal(value) { type = "Literal"; }

    void LiteralNode::print() const {
        std::visit([](const auto& val) {
            std::cout << "[LiteralNode] " << val << "\n";
        }, literal);
    }

    SymbolNode::SymbolNode(std::string symbol) : symbol_name(symbol) { type = "Symbol"; }

    void SymbolNode::print() const {
        std::cout << "[SymbolNode] " << symbol_name << "\n";
    }

    ListNode::ListNode(std::vector<ASTNode*> vec_nodes) {
        type = "List";
        sub_nodes = vec_nodes;
    }

    void ListNode::print() const {
        std::cout << "[ListNode] ";
        for (ASTNode* node_ptr : sub_nodes) {
            std::cout << node_ptr->type << " ";
        }
        std::cout << "(End)\n";
    }

    Parser::Parser(std::vector<std::string> token_list) {
        std::queue<std::string> token_queue;
        for (std::string token : token_list) {
            token_queue.push(token);
        }
        if (token_queue.front() != "(")
            throw syntexError((char*)"[parentheses error] parentheses are not well-matched.");
        root = parse_list(token_queue);
        if (!token_queue.empty())
            throw syntexError((char*)"[parentheses error] parentheses are not well-matched.");
    }

    void Parser::print() {
        print_node(root, 0);
    }

    Parser::NodeType Parser::node_type_finder(std::string str) {
        assert(str.length() > 0);
        if (str[0] == '\'' || str[0] == '"') return NodeType::Literal;
        if ('0' <= str[0] && str[0] <= '9') return NodeType::Literal;
        if ((str[0] == '+' || str[0] == '-') && '0' <= str[1] && str[1] <= '9') return NodeType::Literal;
        if (str == "false" || str == "true" || str == "null") return NodeType::Literal;
        return NodeType::Symbol;
    }

    Parser::LiteralType Parser::literal_type_finder(std::string str) {
        if (str == "false" || str == "true") return LiteralType::Bool;
        if (str == "null") return LiteralType::Null;
        if (str[0] == '\'') {
            if (str.length() == 3 && str[2] == '\'') return LiteralType::Char;
            throw syntexError((char*)"[literal format error] given code does not match to required literal format.");
        }
        if (str[0] == '"') {
            if (str.back() == '"') return LiteralType::String;
            throw syntexError((char*)"[literal format error] given code does not match to required literal format.");
        }
        if (str[0] == '-' || str[0] == '+') str.erase(str.begin());
        for (char c : str) {
            if (c < '0' || c > '9') {
                throw syntexError((char*)"[literal format error] given code does not match to required literal format.");
            }
        }
        return LiteralType::Int;
    }

    int Parser::text_to_int(std::string text) {
        int ret = 0, sgn = 1;
        if (text[0] == '-') sgn = -1;
        if (text[0] == '-' || text[0] == '+') text.erase(text.begin());
        for (char c : text) {
            ret = ret * 10 + (c - '0');
        }
        return sgn * ret;
    }

    char Parser::text_to_char(std::string text) {
        return text[1];
    }

    std::string Parser::text_to_string(std::string text) {
        text.erase(text.begin());
        text.pop_back();
        return text;
    }

    bool Parser::text_to_bool(std::string text) {
        return text == "true";
    }

    std::nullptr_t Parser::text_to_null(std::string text) {
        return nullptr;
    }

    ListNode* Parser::parse_list(std::queue<std::string>& token_queue) {
        std::vector<ASTNode*> childs;
        while (!token_queue.empty()) {
            std::string token = token_queue.front();
            token_queue.pop();
            if (token == ")") {
                return new ListNode(childs);
            }
            if (token == "(") {
                childs.push_back(this->parse_list(token_queue));
            } else {
                NodeType node_type = node_type_finder(token);
                if (node_type == NodeType::Symbol) {
                    childs.push_back(new SymbolNode(token));
                } else if (node_type == NodeType::Literal) {
                    LiteralType literal_type = literal_type_finder(token);
                    if (literal_type == LiteralType::Int)
                        childs.push_back(new LiteralNode(text_to_int(token)));
                    else if (literal_type == LiteralType::Char)
                        childs.push_back(new LiteralNode(text_to_char(token)));
                    else if (literal_type == LiteralType::String)
                        childs.push_back(new LiteralNode(text_to_string(token)));
                    else if (literal_type == LiteralType::Bool)
                        childs.push_back(new LiteralNode(text_to_bool(token)));
                    else if (literal_type == LiteralType::Null)
                        childs.push_back(new LiteralNode(text_to_null(token)));
                }
            }
        }
        throw syntexError((char*)"[parentheses error] parentheses are not well-matched.");
    }

    void Parser::print_node(ASTNode* node, int depth) {
        for (int i = 0; i < depth; i++) std::cout << "    ";
        node->print();
        for (ASTNode* sub : node->sub_nodes) {
            print_node(sub, depth + 1);
        }
    }

    std::vector<std::string> tokenize(std::string str) {
        std::vector<std::string> token_list;
        std::string::iterator start, iter;
        start = str.begin();
        while (start != str.end()) {
            while (start != str.end() && *start == ' ') start++;
            while (start != str.end() && (*start == '(' || *start == ')')) {
                token_list.push_back(std::string(1, *start));
                start++;
            }
            iter = start;
            token_list.push_back(std::string());
            while (iter != str.end() && *iter != ' ' && *iter != '(' && *iter != ')') {
                token_list.back().push_back(*iter);
                iter++;
            }
            if (token_list.back().length() == 0) token_list.pop_back();
            start = iter;
        }

        return token_list;
    }

    Parser read_str(std::string str) {
        std::vector<std::string> token_list = tokenize(str);
        return Parser(token_list);
    }

} // namespace lisp