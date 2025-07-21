#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>

namespace lisp {

    class SyntaxError : public std::exception {
    private:
        char* specific_reason;
    public:
        SyntaxError(char* spec);
        const char* what() const noexcept override;
    };

} // namespace lisp

#endif