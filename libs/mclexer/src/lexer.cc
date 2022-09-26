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

void mclexer::Lexer::pushTokenWhenWordIsPresent(std::vector<mclexer::Token>* tokens) {
    if (!currentWord.empty()) {
        int tokenColumn = column - currentWord.length();
        mclexer::Token token;

        for (auto factory : mclexer::IWordTokenFactory::factories) {
            factory->matchToken(&token, &currentWord, mclexer::TokenLocation(line, tokenColumn));

            if (token.found) {
                tokens->push_back(token);
                currentWord = "";
                return;
            }
        }
    }
}

bool mclexer::Lexer::pushTokenFromCurrentChar(std::vector<mclexer::Token>* tokens, char* currentChar) {
    mclexer::Token token;

    for (auto factory : mclexer::ISingleCharTokenFactory::factories) {
        factory->matchToken(&token, &*currentChar, mclexer::TokenLocation(line, column));

        if (token.found) {
           this->pushTokenWhenWordIsPresent(tokens);
           tokens->push_back(token);

           return true;
        }
    }

    return false;
}

std::unique_ptr<std::vector<mclexer::Token>> mclexer::Lexer::tokenize() {
    auto tokens = std::make_unique<std::vector<mclexer::Token>>();

    reset();

    for (int charIndex = 0; charIndex < source->length(); charIndex++) {
        auto currentCharIterator = source->at(charIndex);
        nextColumn();

        if (this->pushTokenFromCurrentChar(tokens.get(), &currentCharIterator)) {
            continue;
        }

        if (currentCharIterator == '\n') {
            this->pushTokenWhenWordIsPresent(tokens.get());
            nextLine();
            continue;
        }

        if (currentCharIterator == ' ' || currentCharIterator == ',') {
            this->pushTokenWhenWordIsPresent(tokens.get());
            continue;
        }

        currentWord.push_back(currentCharIterator);
    }

    this->pushTokenWhenWordIsPresent(tokens.get());
    return std::move(tokens);
}
