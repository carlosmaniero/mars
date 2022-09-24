// Copyright 2022 Maniero

#include "mclexer/lexer.h"
#include <iostream>

lexer::TokenLocation::TokenLocation(int line, int column) : line { line }, column { column } {}

lexer::Token::Token(TokenLocation location, TokenKind kind, std::string value) :
    location { location }, kind { kind }, value { value } {}

lexer::Token* lexer::SingleCharTokenFactory::makeToken(char* value, TokenLocation tokenLocation) {
    return NULL;
}

lexer::Token* lexer::StatementStartTokenFactory::makeToken(char* value, TokenLocation tokenLocation) {
    if (*value == '(') {
        return new Token(tokenLocation, token_statement_start, "(");
    }
    return NULL;
}

lexer::Token* lexer::StatementEndTokenFactory::makeToken(char* value, TokenLocation tokenLocation) {
    if (*value == ')') {
        return new Token(tokenLocation, token_statement_end, ")");
    }
    return NULL;
}

lexer::Lexer::Lexer(std::string* source) : source { source } {
    this->singleCharTokenFactories = {
      new StatementStartTokenFactory(),
      new StatementEndTokenFactory()
    };
}

void lexer::Lexer::nextLine() {
    line++;
    column = 0;
}

void lexer::Lexer::nextColumn() {
    column++;
}

void lexer::Lexer::reset() {
    line = 1;
    column = 0;
    tokens = {};
    currentWord = "";
}

void lexer::Lexer::makeTokenWithWordIsPresent() {
    if (!currentWord.empty()) {
        int tokenColumn = column - currentWord.length();
        Token token(TokenLocation(line, tokenColumn), token_identifier, currentWord);
        tokens.push_back(token);
    }
    currentWord = "";
}

lexer::Token* lexer::Lexer::nextTokenFromCurrentChar(char* currentChar) {
    for (SingleCharTokenFactory *factory : this->singleCharTokenFactories) {
        Token* singleCharToken = (*factory).makeToken(&*currentChar, TokenLocation(line, column));

        if (singleCharToken != NULL) {
            return singleCharToken;
        }
    }
    return NULL;
}

std::vector<lexer::Token> lexer::Lexer::tokenize() {
    reset();

    std::string:: iterator currentCharIterator;

    for (currentCharIterator = (*source).begin();
         currentCharIterator != (*source).end();
         currentCharIterator++) {
        nextColumn();

        Token* singleCharToken = this->nextTokenFromCurrentChar(&*currentCharIterator);

        if (singleCharToken != NULL) {
            this->makeTokenWithWordIsPresent();
            tokens.push_back(*singleCharToken);

            continue;
        }

        if (*currentCharIterator == '\n') {
            this->makeTokenWithWordIsPresent();
            nextLine();
            continue;
        }

        if (*currentCharIterator == ' ') {
            this->makeTokenWithWordIsPresent();
            continue;
        }


        currentWord.push_back(*currentCharIterator);
    }

    return tokens;
}
