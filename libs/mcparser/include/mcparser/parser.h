// Copyright 2022 Maniero

#ifndef LIBS_MCPARSER_INCLUDE_MCPARSER_PARSER_H_
#define LIBS_MCPARSER_INCLUDE_MCPARSER_PARSER_H_
#include <vector>
#include "mclexer/token.h"
#include "mcparser/context.h"

namespace mcparser {
class Parser {
 public:
     explicit Parser(mcparser::ParserContext* context, std::vector<mclexer::Token>* tokens):
         parserContext {context}, tokens {tokens} {};
 private:
     mcparser::ParserContext* parserContext;
     std::vector<mclexer::Token>* tokens;
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_PARSER_H_
