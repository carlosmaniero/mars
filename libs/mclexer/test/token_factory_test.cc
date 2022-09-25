// Copyright 2022 Maniero

#include <gtest/gtest.h>
#include "mclexer/lexer.h"
#include "mclexer/token.h"

void compareTokens(mctoken::Token token1, mctoken::Token token2) {
    EXPECT_EQ(token1.value, token2.value);
    EXPECT_EQ(token1.kind, token2.kind);
    EXPECT_EQ(token1.location.line, token2.location.line);
    EXPECT_EQ(token1.location.column, token2.location.column);
}

TEST(KeywordTokenFactory, ReturnsATokenForAllTheSymbols) {
    std::string source = "";

    mctokenfactory::KeywordTokenFactory keywordFactory;

    for (auto symbol : mctoken::keywords) {
        mctoken::Token token;
        keywordFactory.matchToken(&token, &symbol, mctoken::TokenLocation(1, 1));

        EXPECT_TRUE(token.found);
    }

    std::string notAKeyword = "loló";

    mctoken::Token token;
    keywordFactory.matchToken(&token, &notAKeyword, mctoken::TokenLocation(1, 1));

    EXPECT_FALSE(token.found);
}
