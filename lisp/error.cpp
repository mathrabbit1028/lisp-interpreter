#include "error.hpp"

namespace lisp {
    
    /* SyntaxError */
    SyntaxError::SyntaxError(char* spec) : specific_reason(spec) {}

    const char* SyntaxError::what() const noexcept {
        return specific_reason;
    }

} // namespace lisp