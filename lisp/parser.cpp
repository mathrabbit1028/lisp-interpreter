#include "parser.hpp"

namespace lisp {
    class syntexError : public std::exception {
    private:
        char* specific_reason;
    public:
        syntexError(char* spec) : specific_reason(spec) {}
        const char* what() const noexcept override {
            return specific_reason;
        }
    };

    class ASTNode {
    public:
        std::string type;
        virtual ~ASTNode() = default;
        virtual void print() const = 0;
    };

    typedef std::variant<int, char, std::string, bool, std::nullptr_t> Literal;
    class LiteralNode : public ASTNode {
    private:
        std::string type = "Literal";
        Literal literal;
    public:
        LiteralNode(int value) : literal(value) {}
        LiteralNode(char value) : literal(value) {}
        LiteralNode(std::string value) : literal(value) {}
        LiteralNode(bool value) : literal(value) {}
        LiteralNode(std::nullptr_t value) : literal(value) {}
        void print() const override {
            std::visit([](const auto& val) {
                std::cout << "[LiteralNode] " << val << "\n";
            }, literal);
        }
    };

    class SymbolNode : public ASTNode {
    private:
        std::string type = "Symbol";
        std::string symbol_name;
    public:
        SymbolNode(std::string symbol) : symbol_name(symbol) {}
        void print() const override {
            std::cout << "[SymbolNode] " << symbol_name << "\n";
        }
    };

    class ListNode : public ASTNode {
    private:
        std::string type = "List";
        std::vector<ASTNode*> sub_nodes;
    public:
        ListNode(std::vector<ASTNode*> vec_nodes) : sub_nodes(vec_nodes) {}
        void print() const override {
            std::cout << "[ListNode] ";
            for (ASTNode* node_ptr : sub_nodes) {
                std::cout << node_ptr->type << " ";
            }
           
            std::cout << "(End)\n";
        }
    };

    class Parser {
    private:
        ASTNode* root;
        bool syntex = true;

        enum class NodeType {
            Literal,
            Symbol,
        };
        NodeType node_type_finder(std::string str) {
            assert(str.length() > 0);
            if (str[0] == '\'' || str[0] == '\"') return NodeType::Literal;
            if ('0' <= str[0] && str[0] <= '9') return NodeType::Literal;
            if ((str[0] == '+' || str[0] == '-') && '0' <= str[1] && str[1] <= '9') return NodeType::Literal;
            if (str == "false" || str == "true" || str == "null") return NodeType::Literal;
            return NodeType::Symbol;
        }
    
        enum class LiteralType {
            Int,
            Char,
            String,
            Bool,
            Null,
        };
        LiteralType literal_type_finder(std::string str) {
            if (str == "false" || str == "true") return LiteralType::Bool;
            if (str == "null") return LiteralType::Null;
            if (str[0] == '\'') {
                if (str.length() == 2 && str[2] == '\'') return LiteralType::Char;
                throw syntexError("[literal format error] given code does not match to required literal format.");
            }
            if (str[0] == '\"') {
                if (str.back() == '\"') return LiteralType::String;
                throw syntexError("[literal format error] given code does not match to required literal format.");
            }
            if (str[0] == '-' || str[0] == '+') str.erase(str.begin());
            for (char c : str) {
                if (c < '0' || c > '9') {
                    throw syntexError("[literal format error] given code does not match to required literal format.");
                }
            }
            return LiteralType::Int;
        }

        int text_to_int(std::string text) {
            int ret = 0, sgn = 1;
            if (text[0] == '-') sgn = -1;
            if (text[0] == '-' || text[0] == '+') text.erase(text.begin());
            for (char c : text) {
                ret += sgn * (c - '0');
                ret *= 10;
            }
            return ret;
        }
        char text_to_char(std::string text) {
            return text[1];
        }
        std::string text_to_string(std::string text) {
            text.erase(text.begin());
            text.erase(text.end());
            return text;
        }
        bool text_to_bool(std::string text) {
            if (text == "true") return true;
            else return false;
        }
        std::nullptr_t text_to_null(std::string text) {
            return nullptr;
        }

        ListNode* parse_list(std::queue<std::string>& token_queue) {
            std::vector<ASTNode*> childs;
            
            while (!token_queue.empty()) {
                std::string token = token_queue.front();
                token_queue.pop();

                if (token == ")") return new ListNode(childs);

                if (token == "(") {
                    childs.push_back(this->parse_list(token_queue));
                }
                else {
                    NodeType node_type = node_type_finder(token);
                    if (node_type == NodeType::Symbol) {
                        childs.push_back(new SymbolNode(token));
                    }
                    else if (node_type == NodeType::Literal) {
                        LiteralType literal_type = literal_type_finder(token);
                        if (literal_type == LiteralType::Int)
                            childs.push_back(new LiteralNode(text_to_int(token)));
                        if (literal_type == LiteralType::Char)
                            childs.push_back(new LiteralNode(text_to_char(token)));
                        if (literal_type == LiteralType::String)
                            childs.push_back(new LiteralNode(text_to_string(token)));
                        if (literal_type == LiteralType::Int)
                            childs.push_back(new LiteralNode(text_to_bool(token)));
                        if (literal_type == LiteralType::Int)
                            childs.push_back(new LiteralNode(text_to_null(token)));
                    }
                }
            }

            throw syntexError("[parentheses error] parentheses are not well-matched.");
        }
    
    public:
        Parser(std::vector<std::string> token_list) {
            std::queue<std::string> token_queue;
            for (std::string token : token_list) {
                token_queue.push(token);
            }

            root = parse_list(token_queue);

            if (!token_queue.empty())
                throw syntexError("[parentheses error] parentheses are not well-matched.");
        }
    };

    std::vector<std::string> tokenize(std::string str) {
        std::vector<char> split_chars = {' '};

        std::vector<std::string> token_list;
        std::string::iterator start, iter;
        while (start != str.end()) {
            if (*start == ' ') start++;
            iter = start;
            token_list.push_back(std::string());
            while (iter != str.end() && *iter != ' ') {
                token_list.back().push_back(*iter);
                iter++;
            }
            start = iter;
        }

        return token_list;
    }

    Parser read_str(std::string str) {
        std::vector<std::string> token_list = tokenize(str);
        return Parser(token_list);
    }
}