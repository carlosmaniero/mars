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

void mclexer::Lexer::nextTokenWhenWordIsPresent(mctoken::Token* token) {
    if (!currentWord.empty()) {
        int tokenColumn = column - currentWord.length();

        for (auto factory : mctokenfactory::IWordTokenFactory::factories) {
            factory->matchToken(token, &currentWord, mctoken::TokenLocation(line, tokenColumn));

            if (token->found) {
                currentWord = "";
                return;
            }
        }
    }
}

void mclexer::Lexer::nextTokenFromCurrentChar(mctoken::Token* token, char* currentChar) {
    for (auto factory : mctokenfactory::ISingleCharTokenFactory::factories) {
        factory->matchToken(token, &*currentChar, mctoken::TokenLocation(line, column));

        if (token->found) {
           return;
        }
    }
}

std::vector<mctoken::Token> mclexer::Lexer::tokenize() {
    reset();

    std::string:: iterator currentCharIterator;

    for (currentCharIterator = (*source).begin(); currentCharIterator != (*source).end(); currentCharIterator++) {
        nextColumn();

        mctoken::Token wordToken;
        mctoken::Token singleCharToken;

        this->nextTokenFromCurrentChar(&singleCharToken, &*currentCharIterator);

        if (singleCharToken.found) {
            this->nextTokenWhenWordIsPresent(&wordToken);

            if (wordToken.found) {
                tokens.push_back(wordToken);
            }

            tokens.push_back(singleCharToken);

            continue;
        }

        if (*currentCharIterator == '\n') {
            this->nextTokenWhenWordIsPresent(&wordToken);

            if (wordToken.found) {
                tokens.push_back(wordToken);
            }

            nextLine();
            continue;
        }

        if (*currentCharIterator == ' ') {
            this->nextTokenWhenWordIsPresent(&wordToken);

            if (wordToken.found) {
                tokens.push_back(wordToken);
            }

            continue;
        }


        currentWord.push_back(*currentCharIterator);
    }

    return tokens;
}
