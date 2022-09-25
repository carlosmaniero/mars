// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
#include <memory>
#include <string>
#include <vector>

#include "mclexer/token.h"
#include "mclexer/token_factory.h"

namespace mclexer {
class Lexer {
 public:
    std::string* source;
    std::unique_ptr<std::vector<mctoken::Token>> tokenize();

    explicit Lexer(std::string* source);
 private:
    std::string currentWord;

    int column;
    int line;

    void reset();
    void nextLine();
    void nextColumn();

    bool pushTokenFromCurrentChar(std::vector<mctoken::Token>* tokens, char* currentChar);
    void pushTokenWhenWordIsPresent(std::vector<mctoken::Token>* tokens);
};
}  // namespace mclexer

#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
