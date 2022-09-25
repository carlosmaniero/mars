// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_H_
#include <string>

namespace mctoken {
static std::string const keywords[] = { // NOLINT
  "fun",
  "method",
  "class",
  "type"
};

enum TokenKind {
  token_eof,
  token_statement_start,
  token_statement_end,
  token_string,
  token_keyword,
  token_number,
  token_identifier
};

class TokenLocation {
 public:
    int line;
    int column;

    TokenLocation(int line, int column);
};

class Token {
 public:
    TokenLocation location;
    TokenKind kind;
    std::string value;
    bool found;

    Token();
    Token(TokenLocation location, TokenKind kind, std::string value);

    void update(TokenLocation location, TokenKind kind, std::string value);
};
}  // namespace mctoken


#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_H_
