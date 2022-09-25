// Copyright 2022 Maniero

#include "mclexer/token_factory.h"
#include "mclexer/token.h"

MATCH_TOKEN(ISingleCharTokenFactory)(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation) {
}

MATCH_TOKEN(StatementStartTokenFactory)(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation) {
    if (*value == '(') {
        token->update(tokenLocation, mctoken::token_statement_start, "(");
    }
}

MATCH_TOKEN(StatementEndTokenFactory)(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation) {
    if (*value == ')') {
        token->update(tokenLocation, mctoken::token_statement_end, ")");
    }
}

std::vector<mctokenfactory::ISingleCharTokenFactory*> mctokenfactory::ISingleCharTokenFactory::factories = {
  new StatementStartTokenFactory(),
  new StatementEndTokenFactory(),
};

MATCH_TOKEN(IWordTokenFactory)(mctoken::Token* token, std::string* word, mctoken::TokenLocation tokenLocation) {
}

MATCH_TOKEN(NumberTokenFactory)(mctoken::Token* token, std::string* word, mctoken::TokenLocation tokenLocation) {
    if (isdigit(word->at(0))) {
        token->update(tokenLocation, mctoken::token_number, *word);
    }
}

MATCH_TOKEN(KeywordTokenFactory)(mctoken::Token* token, std::string* word, mctoken::TokenLocation tokenLocation) {
    for (auto keyword : mctoken::keywords) {
        if (*word == keyword) {
            token->update(tokenLocation, mctoken::token_keyword, *word);
            break;
        }
    }
}

MATCH_TOKEN(IdentifierTokenFactory)(mctoken::Token* token, std::string* word, mctoken::TokenLocation tokenLocation) {
    token->update(tokenLocation, mctoken::token_identifier, *word);
}

std::vector<mctokenfactory::IWordTokenFactory*> mctokenfactory::IWordTokenFactory::factories = {
  new NumberTokenFactory,
  new KeywordTokenFactory,
  new IdentifierTokenFactory
};
