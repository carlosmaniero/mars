// Copyright 2022 Maniero

#include "mcparser/parser.h"

MAKE_ERROR::missingNamespaceIdentifier(mclexer::TokenLocation location) {
    return mcparser::ParserError(location, "missing namespace identifier: you must provide a name to your namespace.");
}

MAKE_ERROR::openParenthesisExpected(mclexer::TokenLocation location, std::string found) {
    return mcparser::ParserError(location, "open parenthesis expected: found \"" + found + "\"");
}
