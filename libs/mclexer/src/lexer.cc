#include "mclexer/lexer.h"
#include <iostream>

using namespace std;

using namespace lexer;

TokenLocation::TokenLocation(int line, int column) : line { line }, column { column }
{}

Token::Token(TokenLocation location, TokenKind kind, std::string value) :
    location { location }, kind { kind }, value { value }
{}

Token* SingleCharTokenFactory::makeToken(char* value, TokenLocation tokenLocation)
{
    return NULL;
}

Token* StatementStartTokenFactory::makeToken(char* value, TokenLocation tokenLocation)
{
    if (*value == '(') {
        return new Token(tokenLocation, token_statement_start, "(");
    }
    return NULL;
}

Token* StatementEndTokenFactory::makeToken(char* value, TokenLocation tokenLocation)
{
    if (*value == ')') {
        return new Token(tokenLocation, token_statement_end, ")");
    }
    return NULL;
}

Lexer::Lexer(std::string* source) : source { source }
{
    this->singleCharTokenFactories = {
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
    tokens = {};
    currentWord = "";
}

void Lexer::makeTokenWithWordIsPresent()
{
    if (!currentWord.empty())
    {
        Token token(TokenLocation(line, column - currentWord.length()), token_identifier, currentWord);
        tokens.push_back(token);
    }
    currentWord = "";
}

Token* Lexer::nextTokenFromCurrentChar(char* currentChar)
{
    for (SingleCharTokenFactory *factory : this->singleCharTokenFactories)
    {
        Token* singleCharToken = (*factory).makeToken(&*currentChar, TokenLocation(line, column));

        if (singleCharToken != NULL)
        {
            return singleCharToken;
        }
    }
    return NULL;
}

std::vector<Token> Lexer::tokenize()
{
    reset();

    std::string:: iterator currentCharIterator;

    for (currentCharIterator = (*source).begin(); currentCharIterator != (*source).end(); currentCharIterator++)
    {
        nextColumn();

        Token* singleCharToken = this->nextTokenFromCurrentChar(&*currentCharIterator);

        if (singleCharToken != NULL)
        {
            this->makeTokenWithWordIsPresent();
            tokens.push_back(*singleCharToken);

            continue;
        }

        if (*currentCharIterator == '\n')
        {
            this->makeTokenWithWordIsPresent();
            nextLine();
            continue;
        }

        if (*currentCharIterator == ' ')
        {
            this->makeTokenWithWordIsPresent();
            continue;
        }


        currentWord.push_back(*currentCharIterator);
    }

    return tokens;
}
