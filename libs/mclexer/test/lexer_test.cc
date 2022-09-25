// Copyright 2022 Maniero

#include <gtest/gtest.h>
#include "mclexer/lexer.h"
#include "mclexer/token.h"

void compareTokens(mctoken::Token token1, mctoken::Token token2) {
    EXPECT_EQ(token1.found, token2.found);
    EXPECT_EQ(token1.value, token2.value);
    EXPECT_EQ(token1.kind, token2.kind);
    EXPECT_EQ(token1.location.line, token2.location.line);
    EXPECT_EQ(token1.location.column, token2.location.column);
}

TEST(LexerTest, ReturnsAnEmptyListGivenAnEmptyInput) {
    std::string source = "";
    mclexer::Lexer my_lexer(&source);
    std::vector<mctoken::Token> tokens;

    my_lexer.tokenize(&tokens);

    EXPECT_EQ(tokens.size(), 0);
}

TEST(LexerTest, ParsesOpenAndCloseStatement) {
    std::string source = "(\n )";
    mclexer::Lexer my_lexer(&source);

    mctoken::TokenLocation start_location(1, 1);
    mctoken::TokenLocation end_location(2, 2);

    mctoken::Token start_token(start_location, mctoken::token_statement_start, "(");
    mctoken::Token end_token(end_location, mctoken::token_statement_end, ")");

    std::vector<mctoken::Token> tokens;
    my_lexer.tokenize(&tokens);

    EXPECT_EQ(tokens.size(), 2);

    compareTokens(tokens.at(0), start_token);
    compareTokens(tokens.at(1), end_token);
}

TEST(LexerTest, ParsesAIdentifier) {
    std::string source = "(hello)";
    mclexer::Lexer my_lexer(&source);

    mctoken::TokenLocation start_location(1, 1);
    mctoken::TokenLocation identifier_location(1, 2);
    mctoken::TokenLocation end_location(1, 7);

    mctoken::Token start_token(start_location, mctoken::token_statement_start, "(");
    mctoken::Token identifier_token(identifier_location, mctoken::token_identifier, "hello");
    mctoken::Token end_token(end_location, mctoken::token_statement_end, ")");

    std::vector<mctoken::Token> tokens;
    my_lexer.tokenize(&tokens);

    EXPECT_EQ(tokens.size(), 3);

    compareTokens(tokens.at(0), start_token);
    compareTokens(tokens.at(1), identifier_token);
    compareTokens(tokens.at(2), end_token);
}

TEST(LexerTest, ParsesMultipleIdentifiers) {
    std::string source = "(a\n b c)";
    mclexer::Lexer my_lexer(&source);

    mctoken::TokenLocation start_location(1, 1);
    mctoken::TokenLocation first_identifier_location(1, 2);
    mctoken::TokenLocation second_identifier_location(2, 2);
    mctoken::TokenLocation third_identifier_location(2, 4);
    mctoken::TokenLocation end_location(2, 5);

    mctoken::Token start_token(start_location, mctoken::token_statement_start, "(");

    mctoken::Token first_identifier_token(first_identifier_location, mctoken::token_identifier, "a");
    mctoken::Token second_identifier_token(second_identifier_location, mctoken::token_identifier, "b");
    mctoken::Token third_identifier_token(third_identifier_location, mctoken::token_identifier, "c");

    mctoken::Token end_token(end_location, mctoken::token_statement_end, ")");

    std::vector<mctoken::Token> tokens;
    my_lexer.tokenize(&tokens);

    EXPECT_EQ(tokens.size(), 5);

    compareTokens(tokens.at(0), start_token);
    compareTokens(tokens.at(1), first_identifier_token);
    compareTokens(tokens.at(2), second_identifier_token);
    compareTokens(tokens.at(3), third_identifier_token);
    compareTokens(tokens.at(4), end_token);
}

TEST(LexerTest, ParsesFunctionSymbol) {
    std::string source = "(fun a)";
    mclexer::Lexer my_lexer(&source);

    mctoken::TokenLocation start_location(1, 1);
    mctoken::TokenLocation function_location(1, 2);
    mctoken::TokenLocation identifier_location(1, 6);
    mctoken::TokenLocation end_location(1, 7);

    mctoken::Token start_token(start_location, mctoken::token_statement_start, "(");

    mctoken::Token function_token(function_location, mctoken::token_keyword, "fun");
    mctoken::Token identifier_token(identifier_location, mctoken::token_identifier, "a");

    mctoken::Token end_token(end_location, mctoken::token_statement_end, ")");

    std::vector<mctoken::Token> tokens;
    my_lexer.tokenize(&tokens);

    EXPECT_EQ(tokens.size(), 4);

    compareTokens(tokens.at(0), start_token);
    compareTokens(tokens.at(1), function_token);
    compareTokens(tokens.at(2), identifier_token);
    compareTokens(tokens.at(3), end_token);
}

TEST(LexerTest, ParsesNumberWhenAnWordStartsWithANumber) {
    std::string source = "13win 13.13 13,45 13";
    mclexer::Lexer my_lexer(&source);

    mctoken::Token firstToken;
    mctoken::Token secondToken;
    mctoken::Token thirdToken;
    mctoken::Token fourthToken;
    mctoken::Token fifthToken;

    firstToken.update(mctoken::TokenLocation(1, 1), mctoken::token_number, "13win");
    secondToken.update(mctoken::TokenLocation(1, 7), mctoken::token_number, "13.13");
    thirdToken.update(mctoken::TokenLocation(1, 13), mctoken::token_number, "13");
    fourthToken.update(mctoken::TokenLocation(1, 16), mctoken::token_number, "45");
    fifthToken.update(mctoken::TokenLocation(1, 18), mctoken::token_number, "13");

    std::vector<mctoken::Token> tokens;
    my_lexer.tokenize(&tokens);

    EXPECT_EQ(tokens.size(), 5);

    compareTokens(tokens.at(0), firstToken);
    compareTokens(tokens.at(1), secondToken);
    compareTokens(tokens.at(2), thirdToken);
    compareTokens(tokens.at(3), fourthToken);
    compareTokens(tokens.at(4), fifthToken);
}
