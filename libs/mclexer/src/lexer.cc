// Copyright 2022 Maniero

#include <iostream>
#include "mclexer/token.h"
#include "mclexer/lexer.h"

mclexer::Lexer::Lexer(std::string* source) : source { source } {}

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
    currentWord = "";
}

void mclexer::Lexer::pushTokenWhenWordIsPresent(std::vector<mctoken::Token>* tokens) {
    if (!currentWord.empty()) {
        int tokenColumn = column - currentWord.length();
        mctoken::Token token;

        for (auto factory : mctokenfactory::IWordTokenFactory::factories) {
            factory->matchToken(&token, &currentWord, mctoken::TokenLocation(line, tokenColumn));

            if (token.found) {
                tokens->push_back(token);
                currentWord = "";
                return;
            }
        }
    }
}

bool mclexer::Lexer::pushTokenFromCurrentChar(std::vector<mctoken::Token>* tokens, char* currentChar) {
    mctoken::Token token;
    for (auto factory : mctokenfactory::ISingleCharTokenFactory::factories) {
        factory->matchToken(&token, &*currentChar, mctoken::TokenLocation(line, column));

        if (token.found) {
           this->pushTokenWhenWordIsPresent(tokens);
           tokens->push_back(token);

           return true;
        }
    }

    return false;
}

void mclexer::Lexer::tokenize(std::vector<mctoken::Token>* tokens) {
    reset();

    std::string:: iterator currentCharIterator;

    for (currentCharIterator = (*source).begin(); currentCharIterator != (*source).end(); currentCharIterator++) {
        nextColumn();

        if (this->pushTokenFromCurrentChar(tokens, &*currentCharIterator)) {
            continue;
        }

        if (*currentCharIterator == '\n') {
            this->pushTokenWhenWordIsPresent(tokens);
            nextLine();
            continue;
        }

        if (*currentCharIterator == ' ') {
            this->pushTokenWhenWordIsPresent(tokens);
            continue;
        }


        currentWord.push_back(*currentCharIterator);
    }
}
