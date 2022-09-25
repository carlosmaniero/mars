// Copyright 2022 Maniero

#ifndef LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_H_
#define LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_H_
#include <string>

namespace mclexer {
static std::string const keywords[] = { // NOLINT
  "fun",
  "method",
  "class",
  "type"
};

enum TokenKind {
  token_eof,
  token_symbol,
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

    void fulfill(TokenLocation location, TokenKind kind, std::string value);
};
}  // namespace mclexer


#endif  // LIBS_MCLEXER_INCLUDE_MCLEXER_TOKEN_H_
