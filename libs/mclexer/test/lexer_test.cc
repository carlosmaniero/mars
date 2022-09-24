// Copyright 2022 Maniero

#include "mclexer/lexer.h"
#include <gtest/gtest.h>

void compare_tokens(lexer::Token token1, lexer::Token token2) {
    EXPECT_EQ(token1.value, token2.value);
    EXPECT_EQ(token1.kind, token2.kind);
    EXPECT_EQ(token1.location.line, token2.location.line);
    EXPECT_EQ(token1.location.column, token2.location.column);
}

TEST(LexerTest, ReturnsAnEmptyListGivenAnEmptyInput) {
    std::string source = "";
    lexer::Lexer my_lexer(&source);

    EXPECT_EQ(my_lexer.tokenize().size(), 0);
}

TEST(LexerTest, ParsesOpenAndCloseStatement) {
    std::string source = "(\n )";
    lexer::Lexer my_lexer(&source);

    lexer::TokenLocation start_location(1, 1);
    lexer::TokenLocation end_location(2, 2);

    lexer::Token start_token(start_location, lexer::token_statement_start, "(");
    lexer::Token end_token(end_location, lexer::token_statement_end, ")");

    std::vector<lexer::Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 2);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), end_token);
}

TEST(LexerTest, ParsesAIdentifier) {
    std::string source = "(hello)";
    lexer::Lexer my_lexer(&source);

    lexer::TokenLocation start_location(1, 1);
    lexer::TokenLocation identifier_location(1, 2);
    lexer::TokenLocation end_location(1, 7);

    lexer::Token start_token(start_location, lexer::token_statement_start, "(");
    lexer::Token identifier_token(identifier_location, lexer::token_identifier, "hello");
    lexer::Token end_token(end_location, lexer::token_statement_end, ")");

    std::vector<lexer::Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 3);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), identifier_token);
    compare_tokens(tokens.at(2), end_token);
}

TEST(LexerTest, ParsesMultipleIdentifiers) {
    std::string source = "(a\n b c)";
    lexer::Lexer my_lexer(&source);

    lexer::TokenLocation start_location(1, 1);
    lexer::TokenLocation first_identifier_location(1, 2);
    lexer::TokenLocation second_identifier_location(2, 2);
    lexer::TokenLocation third_identifier_location(2, 4);
    lexer::TokenLocation end_location(2, 5);

    lexer::Token start_token(start_location, lexer::token_statement_start, "(");

    lexer::Token first_identifier_token(first_identifier_location, lexer::token_identifier, "a");
    lexer::Token second_identifier_token(second_identifier_location, lexer::token_identifier, "b");
    lexer::Token third_identifier_token(third_identifier_location, lexer::token_identifier, "c");

    lexer::Token end_token(end_location, lexer::token_statement_end, ")");

    std::vector<lexer::Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 5);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), first_identifier_token);
    compare_tokens(tokens.at(2), second_identifier_token);
    compare_tokens(tokens.at(3), third_identifier_token);
    compare_tokens(tokens.at(4), end_token);
}
