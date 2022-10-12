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

     static ParserError missingIdentifier(mclexer::Token token);
     static ParserError openParenthesisExpected(mclexer::Token token);
     static ParserError closeParenthesisExpected(mclexer::Token token);
     static ParserError notExpectedToken(mclexer::Token token);
     static ParserError invalidIdentifier(mclexer::Token token);
     static ParserError invalidVisibility(mclexer::Token token);
};

class Parser {
 public:
     std::unique_ptr<mcparser::ASTNode> parse(
         std::unique_ptr<std::vector<mclexer::Token>> tokens);
     std::vector<ParserError> getErrors();

 private:
     int tokenIndex = 0;
     std::vector<ParserError> errors;
     mclexer::Token eatNextToken(std::vector<mclexer::Token>* tokens);
     std::unique_ptr<mclexer::Token> eatNextIdentifierToken(std::vector<mclexer::Token>* tokens);
     bool eatOpenParenthesis(std::vector<mclexer::Token>* tokens);
     std::pair<mcparser::NodeVisibility, bool> parseVisibilityNode(mclexer::Token* token);
     std::unique_ptr<mcparser::NamespaceASTNode> parseNamespace(
         std::vector<mclexer::Token>* tokens);
     std::unique_ptr<mcparser::DefStatementASTNode> parseDef(
         std::vector<mclexer::Token>* tokens);
     std::unique_ptr<mcparser::FunctionStatementASTNode> parseFunction(
         std::vector<mclexer::Token>* tokens);
     std::unique_ptr<mcparser::ASTNode> parseNode(
         std::vector<mclexer::Token>* tokens);
     std::unique_ptr<mcparser::IntegerASTNode> parseInteger(
         std::vector<mclexer::Token>* tokens);
     std::unique_ptr<mcparser::ReferenceIdentifier> parseReference(
         std::vector<mclexer::Token>* tokens);
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_PARSER_H_
