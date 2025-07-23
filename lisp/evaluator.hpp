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
        ASTNode* find(std::string name);
    public:
        Evaluator();
        std::vector<Environment> env_stack;
        Evaluator(Environment globals);
        Literal run(ASTNode* root);
    };
} // namespace lisp

#endif