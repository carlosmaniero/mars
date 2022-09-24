#include "mclexer/lexer.h"
#include <gtest/gtest.h>

using namespace lexer;

void compare_tokens (Token token1, Token token2) {
    EXPECT_EQ(token1.value, token2.value);
    EXPECT_EQ(token1.kind, token2.kind);
    EXPECT_EQ(token1.location.line, token2.location.line);
    EXPECT_EQ(token1.location.column, token2.location.column);
}

TEST(LexerTest, ReturnsAnEmptyListGivenAnEmptyInput)
{
    std::string source = "";
    Lexer my_lexer(&source);

    EXPECT_EQ(my_lexer.tokenize().size(), 0);
}

TEST(LexerTest, ParsesOpenAndCloseStatement)
{
    std::string source = "(\n )";
    Lexer my_lexer(&source);

    TokenLocation start_location(1, 1);
    TokenLocation end_location(2, 2);

    Token start_token(start_location, lexer::token_statement_start, "(");
    Token end_token(end_location, lexer::token_statement_end, ")");

    std::vector<Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 2);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), end_token);
}

TEST(LexerTest, ParsesAIdentifier)
{
    std::string source = "(hello)";
    Lexer my_lexer(&source);

    TokenLocation start_location(1, 1);
    TokenLocation identifier_location(1, 2);
    TokenLocation end_location(1, 7);

    Token start_token(start_location, lexer::token_statement_start, "(");
    Token identifier_token(identifier_location, lexer::token_identifier, "hello");
    Token end_token(end_location, lexer::token_statement_end, ")");

    std::vector<Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 3);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), identifier_token);
    compare_tokens(tokens.at(2), end_token);
}

TEST(LexerTest, ParsesMultipleIdentifiers)
{
    std::string source = "(a\n b c)";
    Lexer my_lexer(&source);

    TokenLocation start_location(1, 1);
    TokenLocation first_identifier_location(1, 2);
    TokenLocation second_identifier_location(2, 2);
    TokenLocation third_identifier_location(2, 4);
    TokenLocation end_location(2, 5);

    Token start_token(start_location, lexer::token_statement_start, "(");

    Token first_identifier_token(first_identifier_location, lexer::token_identifier, "a");
    Token second_identifier_token(second_identifier_location, lexer::token_identifier, "b");
    Token third_identifier_token(third_identifier_location, lexer::token_identifier, "c");

    Token end_token(end_location, lexer::token_statement_end, ")");

    std::vector<Token> tokens = my_lexer.tokenize();

    EXPECT_EQ(tokens.size(), 5);

    compare_tokens(tokens.at(0), start_token);
    compare_tokens(tokens.at(1), first_identifier_token);
    compare_tokens(tokens.at(2), second_identifier_token);
    compare_tokens(tokens.at(3), third_identifier_token);
    compare_tokens(tokens.at(4), end_token);
}
