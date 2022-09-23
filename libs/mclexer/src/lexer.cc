#include "mclexer/lexer.h"
#include <iostream>
using namespace std;

using namespace lexer;

TokenLocation::TokenLocation(int line, int column) : line { line }, column { column }
{}

Token::Token(TokenLocation location, TokenKind kind, std::string value) :
    location { location }, kind { kind }, value { value }
{}

Token* TokenFactory::makeToken(std::string* value, TokenLocation tokenLocation)
{
    return NULL;
}

Token* StatementStartTokenFactory::makeToken(std::string* value, TokenLocation tokenLocation)
{
    if (*value == "(") {
        return new Token(tokenLocation, token_statement_start, "(");
    }
    return NULL;
}

Token* StatementEndTokenFactory::makeToken(std::string* value, TokenLocation tokenLocation)
{
    if (*value == ")") {
        return new Token(tokenLocation, token_statement_end, ")");
    }
    return NULL;
}

Lexer::Lexer(std::string* source) : source { source }
{
    this->tokenFactories = {
      new StatementStartTokenFactory(),
      new StatementEndTokenFactory()
    };
}

void Lexer::nextLine()
{
    line++;
    column = 0;
}

void Lexer::nextColumn()
{
    column++;
}

void Lexer::reset()
{
    line = 1;
    column = 0;
}

Token* Lexer::makeNextToken(std::string* identifier)
{
    for (TokenFactory *factory : this->tokenFactories)
    {
        Token* token = (*factory).makeToken(identifier, TokenLocation(line, column));

        if (token != NULL) {
            return token;
        }
    }
    return NULL;
}

std::vector<Token> Lexer::tokenize()
{
    reset();

    std::vector<Token> tokens;
    std::string identifier = "";
    std::string:: iterator currentCharIterator;

    for (currentCharIterator = (*source).begin(); currentCharIterator != (*source).end(); currentCharIterator++)
    {
        nextColumn();

        if (*currentCharIterator == '\n')
        {
            identifier = "";
            nextLine();
            continue;
        }

        if (*currentCharIterator == ' ')
        {
            identifier = "";
            continue;
        }


        identifier.push_back(*currentCharIterator);

        Token* token = makeNextToken(&identifier);

        if (token != NULL) {
            tokens.push_back(*token);
            identifier = "";
            continue;
        }

    }

    return tokens;
}
