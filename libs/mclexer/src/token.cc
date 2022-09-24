// Copyright 2022 Maniero

#include "mclexer/token.h"

mctoken::TokenLocation::TokenLocation(int line, int column) : line { line }, column { column } {}

mctoken::Token::Token(TokenLocation location, TokenKind kind, std::string value) :
    location { location }, kind { kind }, value { value } {}
