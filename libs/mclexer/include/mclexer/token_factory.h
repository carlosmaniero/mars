// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_

#define MATCH_TOKEN(ARG_CLS) void mclexer::ARG_CLS::matchToken

#include <vector>
#include <string>
#include "mclexer/token.h"

namespace mclexer {
class ISingleCharTokenFactory {
 public:
    static std::vector<ISingleCharTokenFactory*> factories;
    virtual void matchToken(mclexer::Token* token, char* value, mclexer::TokenLocation tokenLocation);
};

class StatementStartTokenFactory : public ISingleCharTokenFactory {
 public:
    void matchToken(mclexer::Token* token, char* value, mclexer::TokenLocation tokenLocation);
};

class StatementEndTokenFactory : public ISingleCharTokenFactory {
 public:
    void matchToken(mclexer::Token* token, char* value, mclexer::TokenLocation tokenLocation);
};

class IWordTokenFactory {
 public:
    static std::vector<IWordTokenFactory*> factories;
    virtual void matchToken(mclexer::Token* token, std::string* value, mclexer::TokenLocation tokenLocation);
};

class NumberTokenFactory : public IWordTokenFactory {
 public:
    void matchToken(mclexer::Token* token, std::string* value, mclexer::TokenLocation tokenLocation);
};

class KeywordTokenFactory : public IWordTokenFactory {
 public:
    void matchToken(mclexer::Token* token, std::string* value, mclexer::TokenLocation tokenLocation);
};

class IdentifierTokenFactory : public IWordTokenFactory {
 public:
    void matchToken(mclexer::Token* token, std::string* value, mclexer::TokenLocation tokenLocation);
};
}  // namespace mclexer

#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_FACTORY_H_
