// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_

#define MATCH_TOKEN(ARG_CLS) void mctokenfactory::ARG_CLS::matchToken

#include <vector>
#include <string>
#include "mclexer/token.h"

namespace mctokenfactory {
class ISingleCharTokenFactory {
 public:
    static std::vector<ISingleCharTokenFactory*> factories;
    virtual void matchToken(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation);
};

class StatementStartTokenFactory : public ISingleCharTokenFactory {
 public:
    void matchToken(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation);
};

class StatementEndTokenFactory : public ISingleCharTokenFactory {
 public:
    void matchToken(mctoken::Token* token, char* value, mctoken::TokenLocation tokenLocation);
};

class IWordTokenFactory {
 public:
    static std::vector<IWordTokenFactory*> factories;
    virtual void matchToken(mctoken::Token* token, std::string* value, mctoken::TokenLocation tokenLocation);
};

class KeywordTokenFactory : public IWordTokenFactory {
 public:
    void matchToken(mctoken::Token* token, std::string* value, mctoken::TokenLocation tokenLocation);
};

class IdentifierTokenFactory : public IWordTokenFactory {
 public:
    void matchToken(mctoken::Token* token, std::string* value, mctoken::TokenLocation tokenLocation);
};
}  // namespace mctokenfactory

#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_
