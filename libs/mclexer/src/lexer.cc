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
    tokens = {};
    currentWord = "";
}

bool mclexer::Lexer::makeTokenWithWordIsPresent(mctoken::Token* token) {
    if (!currentWord.empty()) {
        int tokenColumn = column - currentWord.length();

        for (auto factory : mctokenfactory::IWordTokenFactory::factories) {
            if (factory->makeToken(token, &currentWord, mctoken::TokenLocation(line, tokenColumn))) {
                currentWord = "";
                return true;
            }
        }
    }

    return false;
}

bool mclexer::Lexer::nextTokenFromCurrentChar(mctoken::Token* token, char* currentChar) {
    for (auto factory : mctokenfactory::ISingleCharTokenFactory::factories) {
        if (factory->makeToken(token, &*currentChar, mctoken::TokenLocation(line, column))) {
            return true;
        }
    }
    return false;
}

std::vector<mctoken::Token> mclexer::Lexer::tokenize() {
    reset();

    std::string:: iterator currentCharIterator;

    for (currentCharIterator = (*source).begin();
         currentCharIterator != (*source).end();
         currentCharIterator++) {
        nextColumn();

        mctoken::Token wordToken;
        mctoken::Token singleCharToken;

        if (this->nextTokenFromCurrentChar(&singleCharToken, &*currentCharIterator)) {
            if (this->makeTokenWithWordIsPresent(&wordToken)) {
                tokens.push_back(wordToken);
            }
            tokens.push_back(singleCharToken);

            continue;
        }

        if (*currentCharIterator == '\n') {
            if (this->makeTokenWithWordIsPresent(&wordToken)) {
                tokens.push_back(wordToken);
            }
            nextLine();
            continue;
        }

        if (*currentCharIterator == ' ') {
            if (this->makeTokenWithWordIsPresent(&wordToken)) {
                tokens.push_back(wordToken);
            }
            continue;
        }


        currentWord.push_back(*currentCharIterator);
    }

    return tokens;
}
