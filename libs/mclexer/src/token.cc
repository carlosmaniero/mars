// Copyright 2022 Maniero

#include "mclexer/token.h"

mclexer::TokenLocation::TokenLocation(int line, int column) : line { line }, column { column } {}

mclexer::Token::Token(TokenLocation location, TokenKind kind, std::string value) :
    location { location }, kind { kind }, value { value }, found(true) {}

mclexer::Token::Token() : location(TokenLocation(-1, -1)), found(false) {}

void mclexer::Token::fulfill(TokenLocation location, TokenKind kind, std::string value) {
    this->location = location;
    this->kind = kind;
    this->value = value;
    this->found = true;
}
