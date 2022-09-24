// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
#include <string>
#include <vector>

#include "mclexer/token.h"
#include "mclexer/token_factory.h"

namespace mclexer {
class Lexer {
 public:
    std::string* source;
    std::vector<mctoken::Token> tokenize();

    explicit Lexer(std::string* source);
 private:
    std::string currentWord;
    std::vector<mctoken::Token> tokens;

    int column;
    int line;

    void reset();
    void nextLine();
    void nextColumn();

    bool nextTokenFromCurrentChar(mctoken::Token* token, char* currentChar);
    bool makeTokenWithWordIsPresent(mctoken::Token* token);
};
}  // namespace mclexer

#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
