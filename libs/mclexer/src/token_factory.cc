// Copyright 2022 Maniero

#include "mclexer/token_factory.h"
#include "mclexer/token.h"

MATCH_TOKEN(ISingleCharTokenFactory)(mclexer::Token* token, char* value, mclexer::TokenLocation tokenLocation) {
}

MATCH_TOKEN(StatementStartTokenFactory)(mclexer::Token* token, char* value, mclexer::TokenLocation tokenLocation) {
    if (*value == '(') {
      token->fulfill(tokenLocation, mclexer::token_symbol, "(");
    }
}

MATCH_TOKEN(StatementEndTokenFactory)(mclexer::Token* token, char* value, mclexer::TokenLocation tokenLocation) {
    if (*value == ')') {
        token->fulfill(tokenLocation, mclexer::token_symbol, ")");
    }
}

std::vector<mclexer::ISingleCharTokenFactory*> mclexer::ISingleCharTokenFactory::factories = {
  new StatementStartTokenFactory(),
  new StatementEndTokenFactory(),
};

MATCH_TOKEN(IWordTokenFactory)(mclexer::Token* token, std::string* word, mclexer::TokenLocation tokenLocation) {
}

MATCH_TOKEN(NumberTokenFactory)(mclexer::Token* token, std::string* word, mclexer::TokenLocation tokenLocation) {
    if (isdigit(word->at(0))) {
        token->fulfill(tokenLocation, mclexer::token_number, *word);
    }
}

MATCH_TOKEN(KeywordTokenFactory)(mclexer::Token* token, std::string* word, mclexer::TokenLocation tokenLocation) {
    for (auto keyword : mclexer::keywords) {
        if (*word == keyword) {
            token->fulfill(tokenLocation, mclexer::token_keyword, *word);
            break;
        }
    }
}

MATCH_TOKEN(IdentifierTokenFactory)(mclexer::Token* token, std::string* word, mclexer::TokenLocation tokenLocation) {
    token->fulfill(tokenLocation, mclexer::token_identifier, *word);
}

std::vector<mclexer::IWordTokenFactory*> mclexer::IWordTokenFactory::factories = {
  new NumberTokenFactory,
  new KeywordTokenFactory,
  new IdentifierTokenFactory
};
