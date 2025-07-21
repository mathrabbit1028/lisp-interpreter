#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "astnode.hpp"
#include "environment.hpp"
#include "error.hpp"

#include <vector>
#include <cassert>

namespace lisp {
    class Evaluator {
    private:
        std::vector<Environment> env_stack;
        ASTNode* find(std::string name);
    public:
        Evaluator();
        Evaluator(Environment globals);
        Literal run(ASTNode* root);
    };
} // namespace lisp

#endif