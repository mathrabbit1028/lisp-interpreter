#include "evaluator.hpp"

namespace lisp {

    /*
    void FunctionNode::fix_values(ASTNode* node, Environment* env) {
        if (node->type == "List") {
            for (auto sub_node : ((ListNode*)node)->sub_nodes)
                this->fix_values(sub_node, env);
        }
        else if (node->type == "Symbol") {
            if (this->parameters.find(((SymbolNode*)node)->symbol_name) == this->parameters.end())
                if (env->symbols.find(((SymbolNode*)node)->symbol_name) != env->symbols.end()) 
                    node = env->symbols[((SymbolNode*)node)->symbol_name];
        }
    }
    */

    std::vector<int> __int_binary_operation(std::vector<Literal> argv) {
        std::vector<int> res;
        if (argv.size() != 2)
            throw SyntaxError((char*)"[operator error] Number of operand is not two.");
        for (int i = 0; i < 2; i++) {
            if (!std::holds_alternative<int>(argv[i]))
                throw SyntaxError((char*)"[operator error] Type of operand is not Int.");
            res.push_back(*(std::get_if<int>(&argv[i])));
        }
        return res;
    }

    Literal __add__(std::vector<Literal> argv) {
        std::vector<int> num = __int_binary_operation(argv);
        return num[0] + num[1];
    }

    Literal __sub__(std::vector<Literal> argv) {
        std::vector<int> num = __int_binary_operation(argv);
        return num[0] - num[1];
    }

    Literal __mul__(std::vector<Literal> argv) {
        std::vector<int> num = __int_binary_operation(argv);
        return num[0] * num[1];
    }

    Literal __intdiv__(std::vector<Literal> argv) {
        std::vector<int> num = __int_binary_operation(argv);
        return num[0] / num[1];
    }

    // Literal __global__(std::vector<Literal> argv) {
    //     ;
    // }

    // Literal __local__(std::vector<Literal> argv) {
    //     ;
    // }

    Evaluator::Evaluator(Environment globals) {
        this->env_stack.push_back(globals);
    }
    Evaluator::Evaluator() {
        ;
    }

    ASTNode* Evaluator::find(std::string name) {
        for (int i = (int)(this->env_stack.size()) - 1; i >= 0; i--) {
            ASTNode* res = this->env_stack[i].get(name);
            if (res) return res;
        }
        throw SyntaxError((char*)"[undefined symbol error] Included symbol have not been defined.");
    }

    Literal Evaluator::run(ASTNode* node) {
        if (node->type == "Literal") return ((LiteralNode*)node)->literal;
        else if (node->type == "Function") {
            return nullptr; // implemented later
        }
        else if (node->type == "Symbol") {
            ASTNode* now = this->find(((SymbolNode*)node)->symbol_name);
            if (now->type == "Function") {
                return nullptr; // implemented later
            } else {
                assert(now->type == "Literal");
                return ((LiteralNode*)now)->literal;
            }
        }
        else {
            assert(node->type == "List");

            if (((ListNode*)node)->sub_nodes.size() == 0)
                throw SyntaxError((char*)"[list error] List is empty.");
            
            if (((ListNode*)node)->sub_nodes[0]->type == "Literal")
                throw SyntaxError((char*)"[list error] First symbol of a list is not a function.");
            
            if (((ListNode*)node)->sub_nodes[0]->type == "Symbol") {
                SymbolNode* oper = (SymbolNode*)((ListNode*)node)->sub_nodes[0];
                std::vector<Literal> argv;
                for (int i = 1; i < ((ListNode*)node)->sub_nodes.size(); i++)
                    argv.push_back(this->run(((ListNode*)node)->sub_nodes[i]));
                if (oper->symbol_name ==    "+") return __add__(argv);
                if (oper->symbol_name ==    "-") return __sub__(argv);
                if (oper->symbol_name ==    "*") return __mul__(argv);
                if (oper->symbol_name ==    "/") return __intdiv__(argv);
                // if (oper->symbol_name == "def!") return __global__(argv);
                // if (oper->symbol_name == "let*") return __local__(argv);
                throw SyntaxError((char*)"[list error] First symbol of a list is not a function.");
            }
            FunctionNode* func = (FunctionNode*)((ListNode*)node)->sub_nodes[0];
            if (func->parameters.size() != ((ListNode*)node)->sub_nodes.size() - 1)
                throw SyntaxError((char*)"[list error] Mismatch between the number of parameters and the number of input values.");
            
            return nullptr; // implemented later
        }
    }
} // namespace lisp
