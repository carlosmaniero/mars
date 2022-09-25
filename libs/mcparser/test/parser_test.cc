// Copyright 2022 Maniero

#include "mcparser/parser.h"
#include <gtest/gtest.h>

TEST(Parser, InitialSetup) {
  std::vector<mclexer::Token> tokens;

  mcparser::ParserContext context;
  mcparser::Parser parser(&context, &tokens);

  EXPECT_EQ(true, true);
}
