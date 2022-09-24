#ifndef LEXER_H_
#define LEXER_H_
#include <string>
#include <vector>

namespace lexer
{
  enum TokenKind
  {
    token_eof,
    token_statement_start,
    token_statement_end,
    token_string,
    token_keyword,
    token_identifier
  };

  class TokenLocation
  {
    public:
      int line;
      int column;

      TokenLocation(int line, int column);
  };

  class Token
  {
    public:
      TokenLocation location;
      TokenKind kind;
      std::string value;

      Token(TokenLocation location, TokenKind kind, std::string value);
  };

  class SingleCharTokenFactory {
    public:
      virtual Token* makeToken(char* value, TokenLocation tokenLocation);
  };

  class StatementStartTokenFactory : public SingleCharTokenFactory {
    public:
      Token* makeToken(char* value, TokenLocation tokenLocation);
  };

  class StatementEndTokenFactory : public SingleCharTokenFactory {
    public:
      Token* makeToken(char* value, TokenLocation tokenLocation);
  };

  class Lexer
  {
    public:
      std::string* source;
      std::vector<Token> tokenize();

      Lexer(std::string* source);
    private:
      std::vector<SingleCharTokenFactory*> singleCharTokenFactories;
      std::string currentWord;
      std::vector<Token> tokens;

      int column;
      int line;

      void reset();
      void nextLine();
      void nextColumn();

      Token* nextTokenFromCurrentChar(char* currentChar);
      void makeTokenWithWordIsPresent();
  };
}



#endif // LEXER_H_
