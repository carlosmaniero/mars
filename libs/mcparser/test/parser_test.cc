// Copyright 2022 Maniero

#include <gtest/gtest.h>

#include "mclexer/lexer.h"
#include "mclexer/token.h"
#include "mcparser/ast.h"
#include "mcparser/parser.h"

#define EXPECT_EQ_ERRORS(givenError, expectedError) { \
  EXPECT_EQ(givenError.message, expectedError.message);\
  EXPECT_EQ(givenError.location.column, expectedError.location.column);\
  EXPECT_EQ(givenError.location.line, expectedError.location.line);\
}

TEST(Parser, EmptySource) {
  auto tokens = std::make_unique<std::vector<mclexer::Token>>();

  mcparser::Parser parser;

  auto ast = parser.parse(std::move(tokens));

  EXPECT_EQ(ast, nullptr);
  EXPECT_EQ(parser.getErrors().size(), 0);
}

TEST(Parser, DeclaringNamespaces) {
  std::string source = "(namespace my-ns)";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  mcparser::Parser parser;

  auto ast = parser.parse(std::move(tokens));
  auto astValue = reinterpret_cast<mcparser::NamespaceASTNode*>(ast.get());

  EXPECT_NE(ast, nullptr);

  if (ast != nullptr) {
    EXPECT_EQ(astValue->identifier, "my-ns");
    EXPECT_EQ(astValue->nodes.size(), 0);
    EXPECT_EQ(parser.getErrors().size(), 0);
  }
}

TEST(Parser, DeclaringNamespacesWithoutAnIndentifier) {
  std::string source = "(namespace)";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  mcparser::Parser parser;
  auto ast = parser.parse(std::move(tokens));
  auto astValue = reinterpret_cast<mcparser::NamespaceASTNode*>(ast.get());

  EXPECT_EQ(ast, nullptr);
  auto errors = parser.getErrors();

  EXPECT_EQ(errors.size(), 1);

  auto expectedError = mcparser::ParserError::missingIdentifier(
    mclexer::Token(mclexer::TokenLocation(1, 11), mclexer::token_symbol, ")"));

  EXPECT_EQ_ERRORS(errors.at(0), expectedError);
}

TEST(Parser, DeclaringNamespacesWithAnNonIdentifier) {
  std::string source = "(namespace fun)";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  mcparser::Parser parser;
  auto ast = parser.parse(std::move(tokens));
  auto astValue = reinterpret_cast<mcparser::NamespaceASTNode*>(ast.get());

  EXPECT_EQ(ast, nullptr);
  auto errors = parser.getErrors();

  EXPECT_EQ(errors.size(), 1);

  auto expectedError = mcparser::ParserError::invalidIdentifier(
    mclexer::Token(mclexer::TokenLocation(1, 12), mclexer::token_keyword, "fun"));

  EXPECT_EQ_ERRORS(errors.at(0), expectedError);
}

TEST(Parser, DeclaringNamespacesDoesNotStartsWithOpenStatement) {
  std::string source = "\n\n  namespace";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  mcparser::Parser parser;

  auto ast = parser.parse(std::move(tokens));
  auto astValue = reinterpret_cast<mcparser::NamespaceASTNode*>(ast.get());

  EXPECT_EQ(ast, nullptr);
  auto errors = parser.getErrors();

  EXPECT_EQ(errors.size(), 1);

  auto expectedError = mcparser::ParserError::openParenthesisExpected(
    mclexer::Token(mclexer::TokenLocation(3, 3), mclexer::token_keyword, "namespace"));

  EXPECT_EQ_ERRORS(errors.at(0), expectedError);
}

TEST(Parser, DeclareSomethingAfterStatementEnds) {
  std::string source = "(namespace my-ns) outsider";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  mcparser::Parser parser;
  auto ast = parser.parse(std::move(tokens));
  auto astValue = reinterpret_cast<mcparser::NamespaceASTNode*>(ast.get());

  EXPECT_EQ(ast, nullptr);
  auto errors = parser.getErrors();

  EXPECT_EQ(errors.size(), 1);

  auto expectedError = mcparser::ParserError::notExpectedToken(
    mclexer::Token(mclexer::TokenLocation(1, 19), mclexer::token_identifier, "outsider"));

  EXPECT_EQ_ERRORS(errors.at(0), expectedError);
}

TEST(Parser, DefiningPublicIntegers) {
  std::string source =
    "(namespace my-ns" \
    " (def public Integer my-number 13) " \
    " (def private Integer vice-number 45))";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  mcparser::Parser parser;

  auto ast = parser.parse(std::move(tokens));
  auto astValue = reinterpret_cast<mcparser::NamespaceASTNode*>(ast.get());

  EXPECT_NE(ast, nullptr);

  if (ast != nullptr) {
    EXPECT_EQ(parser.getErrors().size(), 0);

    EXPECT_EQ(astValue->nodes.size(), 2);

    auto def = reinterpret_cast<mcparser::DefStatementASTNode*>(astValue->nodes.at(0).get());

    EXPECT_EQ(def->identifier, "my-number");
    EXPECT_EQ(def->visibility, mcparser::node_visibility_public);

    auto value = reinterpret_cast<mcparser::IntegerASTNode*>(def->value.get());
    auto type = reinterpret_cast<mcparser::NativeIntegerType*>(def->type.get());

    EXPECT_EQ(value->value, 13);
    EXPECT_NE(type, nullptr);

    def = reinterpret_cast<mcparser::DefStatementASTNode*>(astValue->nodes.at(1).get());

    EXPECT_EQ(def->identifier, "vice-number");
    EXPECT_EQ(def->visibility, mcparser::node_visibility_private);

    auto value2 = reinterpret_cast<mcparser::IntegerASTNode*>(def->value.get());
    auto type2 = reinterpret_cast<mcparser::NativeIntegerType*>(def->type.get());

    EXPECT_EQ(value2->value, 45);
    EXPECT_NE(type2, nullptr);
  }
}

TEST(Parser, DefiningWithInvalidVisibility) {
  std::string source =
    "(namespace my-ns \n" \
    " (def anything Integer my-number 13))";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  mcparser::Parser parser;

  auto ast = parser.parse(std::move(tokens));
  auto astValue = reinterpret_cast<mcparser::NamespaceASTNode*>(ast.get());

  auto errors = parser.getErrors();

  EXPECT_EQ(errors.size(), 1);

  auto expectedError = mcparser::ParserError::invalidVisibility(
    mclexer::Token(mclexer::TokenLocation(2, 7), mclexer::token_identifier, "anything"));

  EXPECT_EQ_ERRORS(errors.at(0), expectedError);
}

/*TODO: implement this scenario
TEST(Parser, DefiningWithInvalidVisibility) {
  std::string source =
    "(namespace my-ns" \
    " (def public Integer my-number 13) ";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  mcparser::Parser parser;

  auto ast = parser.parse(std::move(tokens));
  auto astValue = reinterpret_cast<mcparser::NamespaceASTNode*>(ast.get());

  auto errors = parser.getErrors();

  EXPECT_EQ(errors.size(), 1);

  auto expectedError = mcparser::ParserError::missingIdentifier(
    mclexer::Token(mclexer::TokenLocation(1, 11), mclexer::token_symbol, ")"));

  EXPECT_EQ_ERRORS(errors.at(0), expectedError);
}
*/
