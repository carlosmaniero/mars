#include "mclexer/lexer.h"
#include <gtest/gtest.h>

using namespace lexer;

void compare_tokens (Token token1, Token token2) {
    EXPECT_EQ(token1.value, token2.value);
    EXPECT_EQ(token1.kind, token2.kind);
    EXPECT_EQ(token1.location.line, token2.location.line);
    EXPECT_EQ(token1.location.column, token2.location.column);
}

TEST(LexerTest, ReturnsAnEmptyListGivenAnEmptyInput) {
    Lexer my_lexer("");

    EXPECT_EQ(my_lexer.tokenize().size(), 0);
}

TEST(LexerTest, ParsesOpenAndCloseStatement) {
    Lexer my_lexer("(\n )");

    TokenLocation start_location(1, 1);
    TokenLocation end_location(2, 2);

    Token start_token(start_location, lexer::token_statement_start, "(");
    Token end_token(end_location, lexer::token_statement_end, ")");

    std::vector<Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 2) << "must return 2 tokens";

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), end_token);
}
