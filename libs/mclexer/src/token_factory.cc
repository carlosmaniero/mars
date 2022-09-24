// Copyright 2022 Maniero

#include "mclexer/token_factory.h"

DEF_MAKE_TOKEN(ISingleCharTokenFactory)(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation) {
    return false;
}

DEF_MAKE_TOKEN(StatementStartTokenFactory)(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation) {
    if (*value == '(') {
        token->kind = mctoken::token_statement_start;
        token->value = "(";
        token->location = tokenLocation;
        return true;
    }
    return false;
}

DEF_MAKE_TOKEN(StatementEndTokenFactory)(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation) {
    if (*value == ')') {
        token->kind = mctoken::token_statement_end;
        token->value = ")";
        token->location = tokenLocation;
        return true;
    }
    return false;
}

std::vector<mctokenfactory::ISingleCharTokenFactory*> mctokenfactory::ISingleCharTokenFactory::factories = {
  new StatementStartTokenFactory(),
  new StatementEndTokenFactory(),
};

DEF_MAKE_TOKEN(IWordTokenFactory)(mctoken::Token* token, std::string* word, mctoken::TokenLocation tokenLocation) {
    return false;
}

DEF_MAKE_TOKEN(KeywordTokenFactory)(mctoken::Token* token, std::string* word, mctoken::TokenLocation tokenLocation) {
    for (auto keyword : mctoken::keywords) {
        if (*word == keyword) {
            token->kind = mctoken::token_keyword;
            token->value = *word;
            token->location = tokenLocation;
            return true;
        }
    }
    return false;
}

DEF_MAKE_TOKEN(IdentifierTokenFactory)(mctoken::Token* token, std::string* word, mctoken::TokenLocation tokenLocation) {
    token->kind = mctoken::token_identifier;
    token->value = *word;
    token->location = tokenLocation;
    return true;
}

std::vector<mctokenfactory::IWordTokenFactory*> mctokenfactory::IWordTokenFactory::factories = {
  new KeywordTokenFactory,
  new IdentifierTokenFactory
};
