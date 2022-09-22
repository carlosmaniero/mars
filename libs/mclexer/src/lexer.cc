#include "mclexer/lexer.h"
#include <iostream>
using namespace std;

using namespace lexer;

TokenLocation::TokenLocation(int line, int column) : line { line }, column { column }
{}

Token::Token(TokenLocation location, TokenKind kind, std::string value) :
    location { location }, kind { kind }, value { value }
{}

Lexer::Lexer(std::string source) : source { source }
{}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    if (source.length() > 0)
    {
        string:: iterator it;

        int column = 1;
        int line = 1;

        for (it = source.begin(); it != source.end(); it++)
        {
            if (*it == '\n')
            {
                column = 1;
                line++;
                continue;
            }

            if (*it == '(')
            {
                tokens.push_back(Token(TokenLocation(line, column), token_statement_start, "("));
            }

            if (*it == ')')
            {
                tokens.push_back(Token(TokenLocation(line, column), token_statement_end, ")"));
            }

            column++;
        }
    }

    return tokens;
}
