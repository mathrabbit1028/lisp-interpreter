# Make LISP interpreter using C++17

**_For peer reviewer: I would appreciate it if you could point out any unexpected behavior or edge cases you may come across during your review. (use issues)_**

_last update: 2025.07.22._

## 1. Implement read and print

File structure:

```
project/
├── lisp/
│   ├── parser.hpp
│   ├── parser.cpp
│   └── lisp.hpp     (<- integrated header file)  
├──main.cpp
```

**New & Modified Files : ALL**

### `main.cpp`

`main.cpp` includes test source code for checking weather implemented compiler well work.

- You have to give an argument that file path of LISP program; actually, it is a `.txt` file now.
  1. **Compilation** command:
    ```
    g++ lisp/parser.cpp main.cpp -o main -std=c++17
    ```
  2. **Running** command:
    ```
    ./main.exe ./code.txt
    ```
- Each return values means:
  - ` 0` : the given file path is accessible.
  - `-1` : the given file path is inaccessible.
- If there are **syntax errors** in the given file, the runtime error occur.  
  - Please check the error message and modify the source code.
  - Implemented errors:
    - [parentheses error] parentheses are not well-matched.
    - [token error] Given code does not match to required ( typename of blank ) format.
    - (More syntax errors will be supplemented as the project progresses.)
- Also, in next week, I will create a CMake file for simplify compilation and execution.

### `lisp/lisp.hpp`
- `lisp/lisp.hpp` is integrated header file; in other word, if you include `lisp/lisp.hpp` file only, all required header files in folder `lisp` are automatically included.
- We use `lisp` namespace for avoiding name collision. If you can guarantee that collisions do not occur, you can use `using namespace lisp;` (But this is not recommanded.)

### `lisp/parser.cpp` and `lisp/parser.hpp`
There are classes in `lisp/parser.hpp` file:
- **`lisp::ASTNode`**
  - Abstract class for integrating all type of nodes.
  - **Subclasses:**
    - `lisp::LiteralNode`
    - `lisp::SymbolNode`
    - `lisp::ListNode`
- **`lisp::LiteralNode`**
  - Use **std::variant** to handle type variance.
  - **Initializer:** `LiteralNode(T value)`
  - **Attributes:**
    - `type`: equal to `Literal`.
    - `literal`: literal value, type is `T`.
  - **Methods:**
    - `print()`: print `literal`, only when `literal` is streamable.
- **`lisp::SymbolNode`**
  - **Initializer:** `SymbolNode(std::string symbol)`
  - **Attributes:**
    - `type`: equal to `Symbol`.
    - `symbol_name`: symbol name, type is `std::string`.
  - **Methods:**
    - `print()`: print `symbol_name`.
- **`lisp::ListNode`**
  - **Initializer:** `ListNode(std::vector<ASTNode*> vec_nodes)`
  - **Attributes:**
    - `type`: equal to `List`.
    - `sub_nodes`: nodes which the code consists of, type is `std::vector<lisp::ASTNode*>`.
  - **Methods:**
    - `print()`: print node type(`type`) of each element in `sub_nodes`.
- **`lisp::Parser`**
  - **Initializer:** `Parser(std::vector<std::string> token_list)`
  - **Attributes:**
    - `root`: pointer of root node in AST; the type is `ASTNode*`.
  - **Methods:**
    - `node_type_finder`: return node type - {Literal, Symbol}.
    - `literal_type_finder`: return literal type - {Int, Char, String, Bool, Null}.
    - `text_to_[]`: type cast to C++ variable and return it.
  - **Important note: AST construction is already complete in initializer of class.**

There are functions in `lisp/parser.hpp` file:
- **`lisp::tokenize`**
  ```
  std::vector<std::string> lisp::tokenize(std::string str)
  ```
  - `str` : string to tokenize
  - This function returns tokens as `std::vector` data structure.
  - The logic of tokenizing is spliting `str` by whitespace.
    - For processing complicated code, this logic will be changed.
- **`lisp::read_str`**
  ```
  lisp::Parser lisp::read_str(std::string str)
  ```
  - `str` : code to interpreting
  - This function returns `lisp::Parser` object with complete AST of input code.

### Some test cases:
- Syntactically correct cases:
```
(+ 1 3)
(print "Hello, World! ()()")
(     +    1    +3    )
('a' * -7 + 6)
(+((1))(((3))))
(1 'a' "a" false null)
("\n")     <- not handling yet
```

- Syntactically incorrect cases:
```
('a")
(*((1)((3)))
(1ab)
(-3c)
```

## 2. Evaluation

File structure:

```
project/
├── lisp/
│   ├── astnode.hpp
│   ├── astnode.cpp
│   ├── error.hpp
│   ├── error.cpp
│   ├── environment.hpp
│   ├── environment.cpp
│   ├── evaluator.hpp
│   ├── evaluator.cpp
│   ├── parser.hpp
│   ├── parser.cpp
│   └── lisp.hpp     (<- integrated header file)  
├──main.cpp
```

### File Restructuring
- Custom error class is moved to `error.hpp/cpp`.
- All node class is moved to `astnode.hpp/cpp`.
- `parser.hpp/cpp` includes only `parser` class.

### Include Dependency Outline
- `error.hpp <- `
- `parser.hpp <- astnode.hpp, error.hpp`
- `astnode.hpp <- `
- `environment.hpp <- astnode.hpp`
- `evaluator.cpp <- environment.hpp, astnode.hpp, error.hpp`

### `main.cpp`

`main.cpp` includes test source code for checking weather implemented compiler well work.

- We use **CMake** to compile conveniently.
  - Initializaion(only first time), **in project root folder,**
    ```
    mkdir build
    cd build
    cmake ..
    ```
  - In every compilation, **in project root folder,**
    - Release:
      ```
      cmake --build ./build --config Release
      ./build/Release/main ./code.txt
      ```
    - Debug:
      ```
      cmake --build ./build --config Debug
      ./build/Debug/main ./code.txt
      ```
- Implemented errors:
  - [parentheses error] parentheses are not well-matched.
  - [token error] Given code does not match to required ( typename of blank ) format.
  - **(new) [undefined symbol error] Included symbol have not been defined.**
  - **(new) [list error] List is empty.**
  - **(new) [list error] First symbol of a list is not a function.**
  - **(new) [list error] Mismatch between the number of parameters and the number of input values.**
  - **(new) [operator error] Number of operand is not (one, two, ...).**
  - **(new) [operator error] Token type of operand is not (Literal, Symbol, List, Function).**
  - **(new) [operator error] Data type of operand is not (Int, Char, String, Bool, Null).**
  - (More syntax errors will be supplemented as the project progresses.)

### `lisp/astnode.cpp` and `lisp/astnode.hpp`

New class(es) and function(s):

- **`lisp::FunctionNode`**
  - Subclass of `lisp::ASTNode`
  - **Initializer:** `FunctionNode(std::vector<lisp::SymbolNode>, std::vector<lisp::ASTNode*>)`
  - **Attributes:**
    - `parameters`: store value of parameters, type is `std::unordered_map<std::string, lisp::LiteralNode>`.
    - `codes`: AST of each lines of function, type is `std::vector<lisp::ASTNode*>`.
  - **Methods:**
    - `print()`: print `parameters` with specific format that indicate this node is function type.

Modified class(es) and function(s):

- `lisp::Parser::node_type_finder`: return node type - {Literal, Symbol, **Function**}. (todo)

### `lisp/environment.cpp` and `lisp/environment.hpp`
There are classes in `lisp/environment.hpp` file:
- **`lisp::Environment`**
  - **Initializer:** `Environment(std::vector<lisp::SymbolNode>, std::vector<lisp::ASTNode*>)`
  - **Attributes:**
    - `symbols`: store meaning of each symbol, type is `std::unordered_map<std::string, lisp::ASTNode*>`.
  - **Methods:**
    - `add(lisp::SymbolNode, lisp::ASTNode*)`: add new key and value to `symbols`.
    - `get(std::string)`: find the given key and return value; if not exists, it return `nullptr`.
    - `print()`: print keys and values of `symbols` and `functions`.

### `lisp/evaluator.cpp` and `lisp/evaluator.hpp`
There are classes in `lisp/evaluator.hpp` file:
- **`lisp::Evaluator`**
  - **Initializer:** `Evaluator(lisp::Environment globals)`
  - **Attributes:**
    - `env_stack`: `stack` of stored environment table, type is `std::vector<lisp::Environment>`
  - **Methods:**
    - `run(lisp::ASTNode*)`: run AST whose root is given parameter.

There are functions in `lisp/evaluator.hpp` file:
- **`lisp::__int_checking`**
  ```
  std::vector<int> lisp::__int_checking(std::vector<lisp::Literal> argv)
  ```
  - `argv` : operands as `lisp::Literal` type.
  - Check type of each element in `argv` is `Int`.
  - If passes, casts each element to `int` and returns them as `std::vector<int>`.
- **`lisp::__add__`**
  ```
  lisp::Literal lisp::__add__(std::vector<lisp::Literal> argv)
  ```
  - `argv` : operands as `lisp::Literal` type.
  - Check type of each element in `argv` is `Int`.
  - If passes, casts each element to `int` and returns results of addition.
- **`lisp::__sub__`**
  ```
  lisp::Literal lisp::__sub__(std::vector<lisp::Literal> argv)
  ```
  - `argv` : operands as `lisp::Literal` type.
  - Check type of each element in `argv` is `Int`.
  - If passes, casts each element to `int` and returns results of subtraction.
- **`lisp::__mul__`**
  ```
  lisp::Literal lisp::__mul__(std::vector<lisp::Literal> argv)
  ```
  - `argv` : operands as `lisp::Literal` type.
  - Check type of each element in `argv` is `Int`.
  - If passes, casts each element to `int` and returns results of multiplication.
- **`lisp::__intdiv__`**
  ```
  lisp::Literal lisp::__intdiv__(std::vector<lisp::Literal> argv)
  ```
  - `argv` : operands as `lisp::Literal` type.
  - Check type of each element in `argv` is `Int`.
  - If passes, casts each element to `int` and returns results of **division between `int` in C.**
    - `-7/2 = -3`.

### Some test cases:
(will be supplemented)
- Syntactically correct cases:
```
(+ 2 3) -> 5
(+ 2 (* 3 4)) -> 14

(def! a 6) -> 6
(def! b (+ a 2)) -> 8
(+ a b) -> 14
(let* (c 2) c) -> 2
```

# Release

## Install
- (Todo) Run installer `install.exe`.

## Dependency
- This complier is working on **C++17 and over**. You have to install g++ complier that can complie C++17.
- This program uses **C++ standard libraries(std)**.
- This program uses CMake to convenience compilation.

## Syntax of my LISP language
- Code:
  - Must start with `(`.
  - Parentheses must be well-matched.
    - Well-mathcing implies the condtion that there must be **exactly one outermost parenthesis block** — a single top-level expression.
- Symbols:
  - Must not include `(, ), ', "`.
  - Must not start with `', ", 0, 1, ..., 9`.
  - Must not start with `+0, +1, ..., +9` and `-0, -1, ..., -9`.
  - Must not use `false, true, null, +, -, *, /, def!, let*`.
- Literals:
  - Integer literal **(32bit signed)**
    - decimal : `-1, 0, 103, +49`
  - Character literal
    - ASCII character : `'\n', '\t', '!', 'a', '3', 'Z', '\"', '\''`
    - Use `'` for declare charactor.
  - String literal
    - `"Hello, world!"`
    - Use `"` for declare string.
  - Boolean literal
    - `true, false`
  - Null literal
    - `null`
- Lists:
  - List must be include one or more tokens.
  - First token of a list must be function(operator).
  - Number of total tokens except first is equal to number of parameters of function(operator).
- Operators
  - Each operator has fixed number of parameters and type of parameters.
  - `+`, `__add__`
    - requires two int operand.
  - `-`, `__sub__`
    - requires two int operand.
  - `*`, `__mul__`
    - requires two int operand.
  - `/`, `__intdiv__`
    - requires two int operand.
  - `def!`, `__global__`
    - requires two operand -- symbol and anytype.
    - defines symbol as given literal in **global scope.**
    - returns given literal(=value of symbol).
  - `let*`, `__local__`
    - requires two operand -- list and anytype.
    - defines **local parameters** using given list by matching adjacent tokens.
      - In this list, odd-th tokens must be symbol.
      - Also, length of this list must be even.
    - returns value of second operand using value of local parameters.
    - (errors)
      - [operator error] First list of let* does not have even size.
      - [operator error] Odd-th value in list of let* is not symbol token.