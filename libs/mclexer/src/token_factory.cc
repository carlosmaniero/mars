// Copyright 2022 Maniero

#include "mclexer/token_factory.h"

DEF_MAKE_TOKEN(SingleCharTokenFactory)(char* value, mctoken::TokenLocation tokenLocation) {
    return NULL;
}

DEF_MAKE_TOKEN(StatementStartTokenFactory)(char* value, mctoken::TokenLocation tokenLocation) {
    if (*value == '(') {
        return new mctoken::Token(tokenLocation, mctoken::token_statement_start, "(");
    }
    return NULL;
}

DEF_MAKE_TOKEN(StatementEndTokenFactory)(char* value, mctoken::TokenLocation tokenLocation) {
    if (*value == ')') {
        return new mctoken::Token(tokenLocation, mctoken::token_statement_end, ")");
    }
    return NULL;
}
