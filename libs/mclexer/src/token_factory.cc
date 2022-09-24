// Copyright 2022 Maniero

#include "mclexer/token_factory.h"

DEF_MAKE_TOKEN(ISingleCharTokenFactory)(char* value, mctoken::TokenLocation tokenLocation) {
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

std::vector<mctokenfactory::ISingleCharTokenFactory*> mctokenfactory::ISingleCharTokenFactory::factories = {
  new StatementStartTokenFactory(),
  new StatementEndTokenFactory(),
};

DEF_MAKE_TOKEN(IWordTokenFactory)(std::string* word, mctoken::TokenLocation tokenLocation) {
    return NULL;
}

DEF_MAKE_TOKEN(KeywordTokenFactory)(std::string* word, mctoken::TokenLocation tokenLocation) {
    for(auto keyword : mctoken::keywords) {
        if (*word == keyword) {
            return new mctoken::Token(tokenLocation, mctoken::token_keyword, *word);
        }
    }
    return NULL;
}

DEF_MAKE_TOKEN(IdentifierTokenFactory)(std::string* word, mctoken::TokenLocation tokenLocation) {
    return new mctoken::Token(tokenLocation, mctoken::token_identifier, *word);
}

std::vector<mctokenfactory::IWordTokenFactory*> mctokenfactory::IWordTokenFactory::factories = {
  new KeywordTokenFactory,
  new IdentifierTokenFactory
};