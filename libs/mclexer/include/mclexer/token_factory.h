// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_

#define DEF_MAKE_TOKEN(ARG_CLS) bool mctokenfactory::ARG_CLS::makeToken

#include <vector>
#include <string>
#include "mclexer/token.h"

namespace mctokenfactory {
class ISingleCharTokenFactory {
 public:
    static std::vector<ISingleCharTokenFactory*> factories;
    virtual bool makeToken(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation);
};

class StatementStartTokenFactory : public ISingleCharTokenFactory {
 public:
    bool makeToken(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation);
};

class StatementEndTokenFactory : public ISingleCharTokenFactory {
 public:
    bool makeToken(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation);
};

class IWordTokenFactory {
 public:
    static std::vector<IWordTokenFactory*> factories;
    virtual bool makeToken(mctoken::Token* token, std::string* value, mctoken::TokenLocation tokenLocation);
};

class KeywordTokenFactory : public IWordTokenFactory {
 public:
    bool makeToken(mctoken::Token* token, std::string* value, mctoken::TokenLocation tokenLocation);
};

class IdentifierTokenFactory : public IWordTokenFactory {
 public:
    bool makeToken(mctoken::Token* token, std::string* value, mctoken::TokenLocation tokenLocation);
};
}  // namespace mctokenfactory

#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_
