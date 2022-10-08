// Copyright 2022 Maniero

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "mclexer/token.h"
#include "mcparser/ast.h"
#include "mcparser/parser.h"

MAKE_ERROR::missingIdentifier(mclexer::Token token) {
    return mcparser::ParserError(token.location, "missing identifier: you must provide an identifier.");
}

MAKE_ERROR::openParenthesisExpected(mclexer::Token token) {
    return mcparser::ParserError(token.location, "open parenthesis expected: found \"" + token.value + "\"");
}

MAKE_ERROR::closeParenthesisExpected(mclexer::Token token) {
    return mcparser::ParserError(token.location, "close parenthesis expected: found \"" + token.value + "\"");
}

MAKE_ERROR::notExpectedToken(mclexer::Token token) {
    return mcparser::ParserError(token.location, "not expected \"" + token.value + "\" token");
}

MAKE_ERROR::invalidIdentifier(mclexer::Token token) {
    return mcparser::ParserError(token.location, "invalid identifier: \"" + token.value + "\" token");
}

MAKE_ERROR::invalidVisibility(mclexer::Token token) {
    return mcparser::ParserError(
        token.location,
        "invalid visibility. Expected (public, private). Found: \"" + token.value + "\" token");
}

std::unique_ptr<mcparser::ASTNode> mcparser::Parser::parse(
    std::unique_ptr<std::vector<mclexer::Token>> tokens
) {
    if (tokens->empty()) {
        return nullptr;
    }

    auto initialToken = this->eatNextToken(tokens.get());

    if (initialToken.kind != mclexer::token_symbol && initialToken.value != "(") {
        this->errors.push_back(
            mcparser::ParserError::openParenthesisExpected(
                initialToken));
        return nullptr;
    }

    // eat namespace
    this->eatNextToken(tokens.get());

    return this->parseNamespace(tokens.get());
}

std::pair<mcparser::NodeVisibility, bool> mcparser::Parser::parseVisibilityNode(mclexer::Token* token) {
    if (token->value == "public") {
        return std::make_pair(node_visibility_public, true);
    } else if (token->value == "private") {
        return std::make_pair(node_visibility_private, true);
    }

    return std::make_pair(node_visibility_public, false);
}

mclexer::Token mcparser::Parser::eatNextToken(std::vector<mclexer::Token>* tokens) {
    return tokens->at(tokenIndex++);
}

std::unique_ptr<mclexer::Token> mcparser::Parser::eatNextIdentifierToken(std::vector<mclexer::Token>* tokens) {
    auto identifier = this->eatNextToken(tokens);

    if (identifier.kind == mclexer::token_symbol && identifier.value == ")") {
        this->errors.push_back(mcparser::ParserError::missingIdentifier(identifier));
        return nullptr;
    }

    if (identifier.kind != mclexer::token_identifier) {
        this->errors.push_back(mcparser::ParserError::invalidIdentifier(identifier));
        return nullptr;
    }

    return std::make_unique<mclexer::Token>(identifier);
}

std::unique_ptr<mcparser::IntegerASTNode> mcparser::Parser::parseInteger(
    std::vector<mclexer::Token>* tokens) {
    auto token = this->eatNextToken(tokens);
    auto intAst = std::make_unique<mcparser::IntegerASTNode>();
    intAst->value = std::stoi(token.value);
    return std::move(intAst);
}

std::unique_ptr<mcparser::ASTNode> mcparser::Parser::parseNode(
    std::vector<mclexer::Token>* tokens) {

    return this->parseInteger(tokens);
}

std::unique_ptr<mcparser::DefStatementASTNode> mcparser::Parser::parseDef(
    std::vector<mclexer::Token>* tokens) {

    auto visibilityToken = this->eatNextToken(tokens);
    auto typeToken = this->eatNextToken(tokens);

    auto identifier = this->eatNextIdentifierToken(tokens);

    if (identifier == nullptr) {
        return nullptr;
    }

    auto def = std::make_unique<mcparser::DefStatementASTNode>();

    def->identifier = identifier->value;

    auto parsedVisibility = this->parseVisibilityNode(&visibilityToken);

    if (!parsedVisibility.second) {
        this->errors.push_back(mcparser::ParserError::invalidVisibility(visibilityToken));
        return nullptr;
    }

    def->visibility = parsedVisibility.first;
    def->type = std::make_shared<mcparser::NativeIntegerType>();
    def->value = this->parseNode(tokens);

    return def;
}

std::unique_ptr<mcparser::NamespaceASTNode> mcparser::Parser::parseNamespace(
    std::vector<mclexer::Token>* tokens) {
    auto namespaceNameToken = this->eatNextIdentifierToken(tokens);

    if (namespaceNameToken == nullptr) {
        return nullptr;
    }

    if (tokens->size() == 6) {
        this->errors.push_back(mcparser::ParserError::notExpectedToken(tokens->at(4)));
        return nullptr;
    }

    auto nodes = std::vector<std::shared_ptr<mcparser::ASTNode>>();

    int openStatementCount = 1;
    while (this->tokenIndex < tokens->size()) {
        auto token = this->eatNextToken(tokens);

        if (token.value == "(") {
            openStatementCount++;
            token = this->eatNextToken(tokens);

            if (token.value == "def") {
                auto def = this->parseDef(tokens);
                nodes.push_back(std::move(def));
            }
        }

        if (token.value == ")") {
            openStatementCount--;
        }

        if (token.value == EOF_TOKEN_VALUE) {
            if (openStatementCount != 0) {
                this->errors.push_back(mcparser::ParserError::closeParenthesisExpected(token));
            }
        }
    }

    auto ast = std::make_unique<mcparser::NamespaceASTNode>();

    ast->identifier = namespaceNameToken->value;
    ast->nodes = nodes;

    return ast;
}

std::vector<mcparser::ParserError> mcparser::Parser::getErrors() {
    return errors;
}
