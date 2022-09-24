// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
#include <string>
#include <vector>

#include "mclexer/token.h"

namespace mclexer {
class SingleCharTokenFactory {
 public:
    virtual mctoken::Token* makeToken(char* value, mctoken::TokenLocation tokenLocation);
};

class StatementStartTokenFactory : public SingleCharTokenFactory {
 public:
    mctoken::Token* makeToken(char* value, mctoken::TokenLocation tokenLocation);
};

class StatementEndTokenFactory : public SingleCharTokenFactory {
 public:
    mctoken::Token* makeToken(char* value, mctoken::TokenLocation tokenLocation);
};

class Lexer {
 public:
    std::string* source;
    std::vector<mctoken::Token> tokenize();

    explicit Lexer(std::string* source);
 private:
    std::vector<SingleCharTokenFactory*> singleCharTokenFactories;
    std::string currentWord;
    std::vector<mctoken::Token> tokens;

    int column;
    int line;

    void reset();
    void nextLine();
    void nextColumn();

    mctoken::Token* nextTokenFromCurrentChar(char* currentChar);
    void makeTokenWithWordIsPresent();
};
}  // namespace mclexer

#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_LEXER_H_
