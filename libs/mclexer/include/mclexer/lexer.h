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
      bool operator==(const TokenLocation &token) const;
  };

  class Token
  {
    public:
      TokenLocation location;
      TokenKind kind;
      std::string value;

      Token(TokenLocation location, TokenKind kind, std::string value);
      bool operator==(const Token &token) const;
  };

  class Lexer
  {
    public:
      std::string source;

      Lexer(std::string source);
      std::vector<Token> tokenize();

  };
}



#endif // LEXER_H_
