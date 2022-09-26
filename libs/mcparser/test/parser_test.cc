// Copyright 2022 Maniero

#include <gtest/gtest.h>

#include "mclexer/lexer.h"
#include "mclexer/token.h"
#include "mcparser/parser.h"

#define EXPECT_EQ_ERRORS(givenError, expectedError) { \
  EXPECT_EQ(givenError.message, expectedError.message);\
  EXPECT_EQ(givenError.location.column, expectedError.location.column);\
  EXPECT_EQ(givenError.location.line, expectedError.location.line);\
}

class TestAST { };

class NamespaceTestAst : public TestAST {
 public:
    std::string namespaceName;
    std::vector<TestAST> children;
};

class TestParserContext : public mcparser::IParserContext<TestAST> {
 public:
    std::unique_ptr<TestAST> buildNamespace(
        std::string namespaceName,
        std::unique_ptr<std::vector<TestAST>> children) {
      auto ast = std::make_unique<NamespaceTestAst>();

      ast->namespaceName = namespaceName;
      ast->children = *children.get();

      return std::move(ast);
    }
};

void compareError(mcparser::ParserError* givenError, mcparser::ParserError* expectedError) {
  EXPECT_EQ(givenError->message, expectedError->message);
  EXPECT_EQ(givenError->location.column, expectedError->location.column);
  EXPECT_EQ(givenError->location.line, expectedError->location.line);
}

TEST(Parser, DeclaringNamespaces) {
  // Given a namespace name
  std::string source = "(namespace my-ns)";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();

  std::unique_ptr<mcparser::IParserContext<TestAST>> context = std::make_unique<TestParserContext>();
  mcparser::Parser<TestAST> parser;

  auto ast = parser.parse(std::move(context), std::move(tokens));
  auto astValue = reinterpret_cast<NamespaceTestAst*>(ast.get());

  EXPECT_NE(ast, nullptr);

  if (ast != nullptr) {
    EXPECT_EQ(astValue->namespaceName, "my-ns");
    EXPECT_EQ(astValue->children.size(), 0);
    EXPECT_EQ(parser.getErrors().size(), 0);
  }
}

TEST(Parser, DeclaringNamespacesWithoutAnIndentifier) {
  std::string source = "(namespace)";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();
  auto context = std::make_unique<TestParserContext>();

  mcparser::Parser<TestAST> parser;
  auto ast = parser.parse(std::move(context), std::move(tokens));
  auto astValue = reinterpret_cast<NamespaceTestAst*>(ast.get());

  EXPECT_EQ(ast, nullptr);
  auto errors = parser.getErrors();

  EXPECT_EQ(errors.size(), 1);

  auto expectedError = mcparser::ParserError::missingNamespaceIdentifier(mclexer::TokenLocation(1, 11));

  EXPECT_EQ_ERRORS(errors.at(0), expectedError);
}

TEST(Parser, DeclaringNamespacesDoesNotStartsWithOpenStatement) {
  std::string source = "\n\n  namespace";
  mclexer::Lexer lexer(&source);

  auto tokens = lexer.tokenize();
  auto context = std::make_unique<TestParserContext>();

  mcparser::Parser<TestAST> parser;

  auto ast = parser.parse(std::move(context), std::move(tokens));
  auto astValue = reinterpret_cast<NamespaceTestAst*>(ast.get());

  EXPECT_EQ(ast, nullptr);
  auto errors = parser.getErrors();

  EXPECT_EQ(errors.size(), 1);

  auto expectedError = mcparser::ParserError::openParenthesisExpected(
    mclexer::TokenLocation(3, 3), "namespace");

  EXPECT_EQ_ERRORS(errors.at(0), expectedError);
}
