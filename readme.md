# Make LISP interpreter using C++17

*last update: 2025.06.26.*

## 1. Implement read and print

File structure:

```
project/
├── lisp/
│   ├── parser.h
│   └── lisp.h     (<- integrated header file)  
├──main.cpp
```

**New & Modified Files : ALL**

### `main.cpp`

`main.cpp` includes test source code for checking weather implemented compiler well work.

- You have to give an argument that file path of LISP program; actually, it is a `.txt` file now.
  1. **Compilation** command:
    ```
    g++ lisp/*.cpp main.cpp -o main -std=c++17
    ```
  2. **Running** command:
    ```
    ./main.exe [-d] ./code.txt
    ```
  - `-d` means the debug option; if you add `-d` to the running command, the output could be more specific.
- Each return values means:
  - ` 0` : the given file path is accessible; and the code runs as normal mode.
  - ` 1` : the given file path is accessible; and the code runs as debug mode.
  - `-1` : the given file path is inaccessible.
- If there are **syntex errors** in the given file, the runtime error occur.  
  - Please check the error message and modify the source code.
  - Implemented errors:
    - parentheses error : parentheses are not well-matched.
    - literal format error : given code does not match to required literal format.
    - (More syntex errors will be supplemented as the project progresses.)
- Also, in next week, I will create a CMake file for simplify compilation and execution.

### `lisp/lisp.hpp`
- `lisp/lisp.hpp` is integrated header file; in other word, if you include `lisp/lisp.hpp` file only, all required header files in folder `lisp` are automatically included.
- We use `lisp` namespace for avoiding name collision. If you can guarantee that collisions do not occur, you can use `using namespace lisp;` (But this is not recommanded.)

### `lisp/parser.cpp` and `lisp/parser.hpp`
There are classes in `lisp/parser.hpp` file:
- **`lisp::ASTNode`**
  - **Attributes:**
    - `type`: represent type of node; there is fixed value in each subclass.
  - **Methods:**
    - `print()`: print information of instance; **virtual** methods
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
    - `token_queue`: tokens of program; the type is `std::queue<std::string>`.
    - `root`: pointer of root node in AST; the type is `ASTNode*`.
  - **Methods:**
    - `node_type_finder`: return node type - {Literal, Symbol}.
    - `literal_type_finder`: return literal type - {Int, Char, String, Bool, Null}.
    - `text_to_[]`: type cast to c++ variable and return it.
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

<!-- ## 2. ~~

- Added errors:
  - undefined symbol error : included symbol have not been defined. -->

# Release

## Install
- (Todo) Run installer `install.exe`.

## Dependency
- This complier is working on **c++17 and over**. You have to install g++ complier that can complie c++17.
- This program uses **c++ standard libraries(std)**. Also, you have to install `bits/stdc++.h`.

## Syntex of my LISP language
- Symbols:
  - Must not start with `', ", 0, 1, ..., 9`.
  - Must not start with `+0, +1, ..., +9` and `-0, -1, ..., -9`.
  - Must not use `false, true, null`.
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
- Operators