// Copyright 2022 Maniero

#include <gtest/gtest.h>
#include "mclexer/lexer.h"
#include "mclexer/token.h"

#define EXPECT_TOKEN_EQ(token1, token2) { \
    EXPECT_EQ(token1.found, token2.found);\
    EXPECT_EQ(token1.value, token2.value);\
    EXPECT_EQ(token1.kind, token2.kind);\
    EXPECT_EQ(token1.location.line, token2.location.line);\
    EXPECT_EQ(token1.location.column, token2.location.column);\
}

void compareTokens(mclexer::Token token1, mclexer::Token token2) {
    EXPECT_EQ(token1.found, token2.found);
    EXPECT_EQ(token1.value, token2.value);
    EXPECT_EQ(token1.kind, token2.kind);
    EXPECT_EQ(token1.location.line, token2.location.line);
    EXPECT_EQ(token1.location.column, token2.location.column);
}

TEST(LexerTest, ReturnsAnEmptyListGivenAnEmptyInput) {
    std::string source = "";
    mclexer::Lexer my_lexer(&source);

    auto tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens->size(), 1);

    auto expectedToken = mclexer::Token(mclexer::TokenLocation(1, 1), mclexer::token_symbol, EOF_TOKEN_VALUE);

    EXPECT_TOKEN_EQ(tokens->at(0), expectedToken);
}

TEST(LexerTest, ParsesOpenAndCloseStatement) {
    std::string source = "(\n )";
    mclexer::Lexer my_lexer(&source);

    mclexer::Token start_token;
    mclexer::Token end_token;

    start_token.fulfill(mclexer::TokenLocation(1, 1), mclexer::token_symbol, "(");
    end_token.fulfill(mclexer::TokenLocation(2, 2), mclexer::token_symbol, ")");
    auto eof_token = mclexer::Token(mclexer::TokenLocation(2, 3), mclexer::token_symbol, EOF_TOKEN_VALUE);

    auto tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens->size(), 3);

    EXPECT_TOKEN_EQ(tokens->at(0), start_token);
    EXPECT_TOKEN_EQ(tokens->at(1), end_token);
    EXPECT_TOKEN_EQ(tokens->at(2), eof_token);
}

TEST(LexerTest, ParsesAIdentifier) {
    std::string source = "(hello)";
    mclexer::Lexer my_lexer(&source);

    mclexer::Token start_token;
    mclexer::Token identifier_token;
    mclexer::Token end_token;

    start_token.fulfill(mclexer::TokenLocation(1, 1), mclexer::token_symbol, "(");
    identifier_token.fulfill(mclexer::TokenLocation(1, 2), mclexer::token_identifier, "hello");
    end_token.fulfill(mclexer::TokenLocation(1, 7), mclexer::token_symbol, ")");
    auto eof_token = mclexer::Token(mclexer::TokenLocation(1, 8), mclexer::token_symbol, EOF_TOKEN_VALUE);

    auto tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens->size(), 4);

    EXPECT_TOKEN_EQ(tokens->at(0), start_token);
    EXPECT_TOKEN_EQ(tokens->at(1), identifier_token);
    EXPECT_TOKEN_EQ(tokens->at(2), end_token);
    EXPECT_TOKEN_EQ(tokens->at(3), eof_token);
}

TEST(LexerTest, ParsesMultipleIdentifiers) {
    std::string source = "(a\n b c)";
    mclexer::Lexer my_lexer(&source);

    mclexer::Token start_token;
    mclexer::Token first_identifier_token;
    mclexer::Token second_identifier_token;
    mclexer::Token third_identifier_token;
    mclexer::Token end_token;

    start_token.fulfill(mclexer::TokenLocation(1, 1), mclexer::token_symbol, "(");
    first_identifier_token.fulfill(mclexer::TokenLocation(1, 2), mclexer::token_identifier, "a");
    second_identifier_token.fulfill(mclexer::TokenLocation(2, 2), mclexer::token_identifier, "b");
    third_identifier_token.fulfill(mclexer::TokenLocation(2, 4), mclexer::token_identifier, "c");
    end_token.fulfill(mclexer::TokenLocation(2, 5), mclexer::token_symbol, ")");
    auto eof_token = mclexer::Token(mclexer::TokenLocation(2, 6), mclexer::token_symbol, EOF_TOKEN_VALUE);

    auto tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens->size(), 6);

    EXPECT_TOKEN_EQ(tokens->at(0), start_token);
    EXPECT_TOKEN_EQ(tokens->at(1), first_identifier_token);
    EXPECT_TOKEN_EQ(tokens->at(2), second_identifier_token);
    EXPECT_TOKEN_EQ(tokens->at(3), third_identifier_token);
    EXPECT_TOKEN_EQ(tokens->at(4), end_token);
    EXPECT_TOKEN_EQ(tokens->at(5), eof_token);
}

TEST(LexerTest, ParsesFunctionSymbol) {
    std::string source = "(fun a)";
    mclexer::Lexer my_lexer(&source);

    mclexer::Token start_token;
    mclexer::Token function_token;
    mclexer::Token identifier_token;
    mclexer::Token end_token;

    start_token.fulfill(mclexer::TokenLocation(1, 1), mclexer::token_symbol, "(");
    function_token.fulfill(mclexer::TokenLocation(1, 2), mclexer::token_keyword, "fun");
    identifier_token.fulfill(mclexer::TokenLocation(1, 6), mclexer::token_identifier, "a");
    end_token.fulfill(mclexer::TokenLocation(1, 7), mclexer::token_symbol, ")");
    auto eof_token = mclexer::Token(mclexer::TokenLocation(1, 8), mclexer::token_symbol, EOF_TOKEN_VALUE);

    auto tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens->size(), 5);

    EXPECT_TOKEN_EQ(tokens->at(0), start_token);
    EXPECT_TOKEN_EQ(tokens->at(1), function_token);
    EXPECT_TOKEN_EQ(tokens->at(2), identifier_token);
    EXPECT_TOKEN_EQ(tokens->at(3), end_token);
    EXPECT_TOKEN_EQ(tokens->at(4), eof_token);
}

TEST(LexerTest, ParsesNumberWhenAnWordStartsWithANumber) {
    std::string source = "13win 13.13 13,45 13";
    mclexer::Lexer my_lexer(&source);

    mclexer::Token firstToken;
    mclexer::Token secondToken;
    mclexer::Token thirdToken;
    mclexer::Token fourthToken;
    mclexer::Token fifthToken;

    firstToken.fulfill(mclexer::TokenLocation(1, 1), mclexer::token_number, "13win");
    secondToken.fulfill(mclexer::TokenLocation(1, 7), mclexer::token_number, "13.13");
    thirdToken.fulfill(mclexer::TokenLocation(1, 13), mclexer::token_number, "13");
    fourthToken.fulfill(mclexer::TokenLocation(1, 16), mclexer::token_number, "45");
    fifthToken.fulfill(mclexer::TokenLocation(1, 19), mclexer::token_number, "13");
    auto eof_token = mclexer::Token(mclexer::TokenLocation(1, 21), mclexer::token_symbol, EOF_TOKEN_VALUE);

    std::vector<mclexer::Token> myRefToken;
    auto tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens->size(), 6);

    EXPECT_TOKEN_EQ(tokens->at(0), firstToken);
    EXPECT_TOKEN_EQ(tokens->at(1), secondToken);
    EXPECT_TOKEN_EQ(tokens->at(2), thirdToken);
    EXPECT_TOKEN_EQ(tokens->at(3), fourthToken);
    EXPECT_TOKEN_EQ(tokens->at(4), fifthToken);
    EXPECT_TOKEN_EQ(tokens->at(5), eof_token);
}
