// Copyright 2022 Maniero

#ifndef LIBS_MCPARSER_INCLUDE_MCPARSER_PARSER_H_
#define LIBS_MCPARSER_INCLUDE_MCPARSER_PARSER_H_
#define MAKE_ERROR mcparser::ParserError mcparser::ParserError
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "mclexer/token.h"
#include "mcparser/context.h"

namespace mcparser {
class ParserError {
 public:
     mclexer::TokenLocation location;
     std::string message;

     ParserError(mclexer::TokenLocation location, std::string message) : location {location}, message { message } {};

     static ParserError missingNamespaceIdentifier(mclexer::TokenLocation tokenLocation);
     static ParserError openParenthesisExpected(mclexer::TokenLocation tokenLocation, std::string found);
};

template <class TAST> class Parser {
 public:
     std::unique_ptr<TAST> parse(
         std::unique_ptr<mcparser::IParserContext<TAST>> parserContext,
         std::unique_ptr<std::vector<mclexer::Token>> tokens
     ) {
         for (int tokenIndex = 0; tokenIndex < tokens->size(); tokenIndex++) {
             if (tokenIndex == 0) {
                 if (tokens->at(0).kind != mclexer::token_symbol && tokens->at(0).value != "(") {
                     this->errors.push_back(
                         mcparser::ParserError::openParenthesisExpected(
                             tokens->at(0).location,
                             tokens->at(0).value));
                     return nullptr;
                 }
             }
         }
         if (tokens->size() != 4) {
             this->errors.push_back(mcparser::ParserError::missingNamespaceIdentifier(mclexer::TokenLocation(1, 11)));
             return nullptr;
         }
         return std::move(parserContext->buildNamespace("my-ns", std::make_unique<std::vector<TAST>>()));
     }

     std::vector<ParserError> getErrors() {
         return errors;
     }

 private:
     std::vector<ParserError> errors;
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_PARSER_H_
