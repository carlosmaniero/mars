// Copyright 2022 Maniero

#include "mclexer/lexer.h"
#include <gtest/gtest.h>

void compare_tokens(mclexer::Token token1, mclexer::Token token2) {
    EXPECT_EQ(token1.value, token2.value);
    EXPECT_EQ(token1.kind, token2.kind);
    EXPECT_EQ(token1.location.line, token2.location.line);
    EXPECT_EQ(token1.location.column, token2.location.column);
}

TEST(LexerTest, ReturnsAnEmptyListGivenAnEmptyInput) {
    std::string source = "";
    mclexer::Lexer my_lexer(&source);

    EXPECT_EQ(my_lexer.tokenize().size(), 0);
}

TEST(LexerTest, ParsesOpenAndCloseStatement) {
    std::string source = "(\n )";
    mclexer::Lexer my_lexer(&source);

    mclexer::TokenLocation start_location(1, 1);
    mclexer::TokenLocation end_location(2, 2);

    mclexer::Token start_token(start_location, mclexer::token_statement_start, "(");
    mclexer::Token end_token(end_location, mclexer::token_statement_end, ")");

    std::vector<mclexer::Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 2);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), end_token);
}

TEST(LexerTest, ParsesAIdentifier) {
    std::string source = "(hello)";
    mclexer::Lexer my_lexer(&source);

    mclexer::TokenLocation start_location(1, 1);
    mclexer::TokenLocation identifier_location(1, 2);
    mclexer::TokenLocation end_location(1, 7);

    mclexer::Token start_token(start_location, mclexer::token_statement_start, "(");
    mclexer::Token identifier_token(identifier_location, mclexer::token_identifier, "hello");
    mclexer::Token end_token(end_location, mclexer::token_statement_end, ")");

    std::vector<mclexer::Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 3);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), identifier_token);
    compare_tokens(tokens.at(2), end_token);
}

TEST(LexerTest, ParsesMultipleIdentifiers) {
    std::string source = "(a\n b c)";
    mclexer::Lexer my_lexer(&source);

    mclexer::TokenLocation start_location(1, 1);
    mclexer::TokenLocation first_identifier_location(1, 2);
    mclexer::TokenLocation second_identifier_location(2, 2);
    mclexer::TokenLocation third_identifier_location(2, 4);
    mclexer::TokenLocation end_location(2, 5);

    mclexer::Token start_token(start_location, mclexer::token_statement_start, "(");

    mclexer::Token first_identifier_token(first_identifier_location, mclexer::token_identifier, "a");
    mclexer::Token second_identifier_token(second_identifier_location, mclexer::token_identifier, "b");
    mclexer::Token third_identifier_token(third_identifier_location, mclexer::token_identifier, "c");

    mclexer::Token end_token(end_location, mclexer::token_statement_end, ")");

    std::vector<mclexer::Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 5);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), first_identifier_token);
    compare_tokens(tokens.at(2), second_identifier_token);
    compare_tokens(tokens.at(3), third_identifier_token);
    compare_tokens(tokens.at(4), end_token);
}
