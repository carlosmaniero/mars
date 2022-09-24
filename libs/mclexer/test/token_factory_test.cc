// Copyright 2022 Maniero

#include <gtest/gtest.h>
#include "mclexer/lexer.h"
#include "mclexer/token.h"

void compare_tokens(mctoken::Token token1, mctoken::Token token2) {
    EXPECT_EQ(token1.value, token2.value);
    EXPECT_EQ(token1.kind, token2.kind);
    EXPECT_EQ(token1.location.line, token2.location.line);
    EXPECT_EQ(token1.location.column, token2.location.column);
}

TEST(KeywordTokenFactory, ReturnsATokenForAllTheSymbols) {
    std::string source = "";

    mctokenfactory::KeywordTokenFactory keywordFactory;
    mctoken::Token token;

    for (auto symbol : mctoken::keywords) {
        EXPECT_TRUE(keywordFactory.makeToken(&token, &symbol, mctoken::TokenLocation(1, 1)));
    }

    std::string notAKeyword = "loló";

    EXPECT_FALSE(keywordFactory.makeToken(&token, &notAKeyword, mctoken::TokenLocation(1, 1)));
}
