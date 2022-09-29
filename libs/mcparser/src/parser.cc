// Copyright 2022 Maniero

#include "mcparser/parser.h"

MAKE_ERROR::missingIdentifier(mclexer::Token token) {
    return mcparser::ParserError(token.location, "missing identifier: you must provide an identifier.");
}

MAKE_ERROR::openParenthesisExpected(mclexer::Token token) {
    return mcparser::ParserError(token.location, "open parenthesis expected: found \"" + token.value + "\"");
}

MAKE_ERROR::notExpectedToken(mclexer::Token token) {
    return mcparser::ParserError(token.location, "not expected \"" + token.value + "\" token");
}

MAKE_ERROR::invalidIdentifier(mclexer::Token token) {
    return mcparser::ParserError(token.location, "invalid identifier: \"" + token.value + "\" token");
}

std::unique_ptr<mcparser::ASTNode> mcparser::Parser::parse(
    std::unique_ptr<mcparser::IParserContext> parserContext,
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

    return std::move(this->parseNamespace(parserContext.get(), tokens.get()));
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
    mcparser::IParserContext* parserContext,
    std::vector<mclexer::Token>* tokens) {
    auto token = this->eatNextToken(tokens);

    return std::move(parserContext->buildInteger(std::stoi(token.value)));
}

std::unique_ptr<mcparser::ASTNode> mcparser::Parser::parseNode(
    mcparser::IParserContext* parserContext,
    std::vector<mclexer::Token>* tokens) {

    return this->parseInteger(parserContext, tokens);
}

std::unique_ptr<mcparser::DefStatementASTNode> mcparser::Parser::parseDef(
    mcparser::IParserContext* parserContext,
    std::vector<mclexer::Token>* tokens) {
    auto identifier = this->eatNextIdentifierToken(tokens);

    if (identifier == nullptr) {
        return nullptr;
    }

    return parserContext->buildDef(
        identifier->value,
        mcparser::node_visibility_public,
        this->parseNode(parserContext, tokens));
}

std::unique_ptr<mcparser::NamespaceASTNode> mcparser::Parser::parseNamespace(
    mcparser::IParserContext* parserContext,
    std::vector<mclexer::Token>* tokens) {
    auto namespaceNameToken = this->eatNextIdentifierToken(tokens);

    if (namespaceNameToken == nullptr) {
        return nullptr;
    }

    if (tokens->size() == 5) {
        this->errors.push_back(mcparser::ParserError::notExpectedToken(tokens->at(4)));
        return nullptr;
    }

    auto nodes = std::vector<std::unique_ptr<mcparser::ASTNode>>();

    while (this->tokenIndex < tokens->size()) {
        auto token = this->eatNextToken(tokens);

        if (token.value == "(") {
            token = this->eatNextToken(tokens);

            if (token.value == "def") {
                auto def = this->parseDef(parserContext, tokens);
                nodes.push_back(std::move(def));
            }
        }
    }

    return std::move(parserContext->buildNamespace(namespaceNameToken->value, std::move(nodes)));
}

std::vector<mcparser::ParserError> mcparser::Parser::getErrors() {
    return errors;
}
