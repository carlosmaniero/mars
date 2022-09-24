// Copyright 2022 Maniero

#include <iostream>
#include "mclexer/token.h"
#include "mclexer/lexer.h"

mclexer::Lexer::Lexer(std::string* source) : source { source } {
    this->singleCharTokenFactories = {
      new mctokenfactory::StatementStartTokenFactory(),
      new mctokenfactory::StatementEndTokenFactory()
    };
}

void mclexer::Lexer::nextLine() {
    line++;
    column = 0;
}

void mclexer::Lexer::nextColumn() {
    column++;
}

void mclexer::Lexer::reset() {
    line = 1;
    column = 0;
    tokens = {};
    currentWord = "";
}

void mclexer::Lexer::makeTokenWithWordIsPresent() {
    if (!currentWord.empty()) {
        int tokenColumn = column - currentWord.length();
        mctoken::Token token(mctoken::TokenLocation(line, tokenColumn), mctoken::token_identifier, currentWord);
        tokens.push_back(token);
    }
    currentWord = "";
}

mctoken::Token* mclexer::Lexer::nextTokenFromCurrentChar(char* currentChar) {
    for (auto *factory : this->singleCharTokenFactories) {
        mctoken::Token* singleCharToken = (*factory).makeToken(&*currentChar, mctoken::TokenLocation(line, column));

        if (singleCharToken != NULL) {
            return singleCharToken;
        }
    }
    return NULL;
}

std::vector<mctoken::Token> mclexer::Lexer::tokenize() {
    reset();

    std::string:: iterator currentCharIterator;

    for (currentCharIterator = (*source).begin();
         currentCharIterator != (*source).end();
         currentCharIterator++) {
        nextColumn();

        mctoken::Token* singleCharToken = this->nextTokenFromCurrentChar(&*currentCharIterator);

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
