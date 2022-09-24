// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_

#define DEF_MAKE_TOKEN(ARG_CLS) mctoken::Token* mctokenfactory::ARG_CLS::makeToken

#include "mclexer/token.h"

namespace mctokenfactory {
class SingleCharTokenFactory {
 public:
    virtual mctoken::Token* makeToken(char* value, mctoken::TokenLocation tokenLocation);
};

class StatementStartTokenFactory : public SingleCharTokenFactory {
 public:
    mctoken::Token* makeToken(char* value, mctoken::TokenLocation tokenLocation);
};

class StatementEndTokenFactory : public SingleCharTokenFactory {
 public:
    mctoken::Token* makeToken(char* value, mctoken::TokenLocation tokenLocation);
};
}  // namespace mctokenfactory

#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_
