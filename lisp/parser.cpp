#include "parser.hpp"

namespace lisp {

    /* Parser */
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
            throw SyntaxError((char*)"[token error] Given code does not match to required character format.");
        }
        if (str[0] == '"') {
            if (str.back() == '"') return LiteralType::String;
            throw SyntaxError((char*)"[token error] Given code does not match to required string format.");
        }
        if (str[0] == '-' || str[0] == '+') str.erase(str.begin());
        for (char c : str) {
            if (c < '0' || c > '9') {
                throw SyntaxError((char*)"[token error] Given code does not match to required integer format.");
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

    ASTNode* Parser::token_to_node(std::string token) {
        NodeType node_type = node_type_finder(token);
        if (node_type == NodeType::Symbol) {
            return new SymbolNode(token);
        } else if (node_type == NodeType::Literal) {
            LiteralType literal_type = literal_type_finder(token);
            if (literal_type == LiteralType::Int)
                return new LiteralNode(text_to_int(token));
            else if (literal_type == LiteralType::Char)
                return new LiteralNode(text_to_char(token));
            else if (literal_type == LiteralType::String)
                return new LiteralNode(text_to_string(token));
            else if (literal_type == LiteralType::Bool)
                return new LiteralNode(text_to_bool(token));
            else if (literal_type == LiteralType::Null)
                return new LiteralNode(text_to_null(token));
        }
        throw SyntaxError((char*)"[token error] Given token does not match to required format.");
    }

    ListNode* Parser::parse_list(std::queue<std::string>& token_queue) {
        std::vector<ASTNode*> childs;
        while (!token_queue.empty()) {
            std::string token = token_queue.front();
            token_queue.pop();
            if (token == ")") {
                return new ListNode(childs);
            } else if (token == "(") {
                childs.push_back(this->parse_list(token_queue));
            } else {
                childs.push_back(token_to_node(token));
            }
        }
        throw SyntaxError((char*)"[parentheses error] Parentheses are not well-matched.");
    }

    Parser::Parser(std::vector<std::string> token_list) {
        std::queue<std::string> token_queue;
        for (std::string token : token_list) {
            token_queue.push(token);
        }

        /* general case
        while (!token_queue.empty()) {
            std::string token = token_queue.front();
            token_queue.pop();
            if (token == ")") {
                throw SyntaxError((char*)"[parentheses error] Parentheses are not well-matched.");
            } else if (token == "(") {
                root->sub_nodes.push_back(this->parse_list(token_queue));
            } else {
                root->sub_nodes.push_back(token_to_node(token));
            }
        } */

        if (token_queue.front() != "(")
            throw SyntaxError((char*)"[parentheses error] Code must start with an opening parenthesis.");
        
        token_queue.pop();
        ((ListNode*)root)->sub_nodes.push_back(this->parse_list(token_queue));
        
        if (!token_queue.empty())
            throw SyntaxError((char*)"[parentheses error] Parentheses are not well-matched.");
    }

    void Parser::print_node(ASTNode* node, int depth) {
        for (int i = 0; i < depth; i++) std::cout << "    ";
        node->print();
        if (node->type == "List") {
            for (ASTNode* sub_node : ((ListNode*)node)->sub_nodes) {
                print_node(sub_node, depth + 1);
            }
        }
    }

    void Parser::print() {
        print_node(root, 0);
    }

    std::vector<std::string> tokenize(std::string str) {
        std::vector<std::string> token_list;
        std::string::iterator start;
        start = str.begin();
        while (start != str.end()) {
            if (start != str.end() && *start == ' ') {
                start++;
                continue;
            }
            if (start != str.end() && *start == '"') {
                token_list.push_back(std::string(1, '"'));
                start++;
                while (start != str.end() && *start != '"') {
                    token_list.back().push_back(*start);
                    start++;
                }
                if (start == str.end()) 
                    throw SyntaxError((char*)"[token error] Given token does not match to required format.");
                token_list.back().push_back(*start);
                start++;
                continue;
            }
            if (start != str.end() && *start == '\'') {
                token_list.push_back(std::string(1, '\''));
                start++;
                while (start != str.end() && *start != '\'') {
                    token_list.back().push_back(*start);
                    start++;
                }
                if (start == str.end()) 
                    throw SyntaxError((char*)"[token error] Given token does not match to required format.");
                token_list.back().push_back(*start);
                start++;
                if (token_list.back().length() != 3) 
                    throw SyntaxError((char*)"[token error] Given token does not match to required format.");
                continue;
            }
            if (start != str.end() && (*start == '(' || *start == ')')) {
                token_list.push_back(std::string(1, *start));
                start++;
                continue;
            }
            std::string::iterator iter = start;
            token_list.push_back(std::string());
            while (iter != str.end() && *iter != ' ' && *iter != '(' && *iter != ')' && *iter != '\'' && *iter != '"') {
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