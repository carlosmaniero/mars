// Copyright 2022 Maniero

#ifndef LIBS_MCPARSER_INCLUDE_MCPARSER_CONTEXT_H_
#define LIBS_MCPARSER_INCLUDE_MCPARSER_CONTEXT_H_
#include <memory>
#include <string>
#include <vector>
#include "mcparser/ast.h"

namespace mcparser {
class IParserContext {
 public:
    virtual std::unique_ptr<mcparser::NamespaceASTNode> buildNamespace(
        std::string namespaceName,
        std::vector<std::unique_ptr<mcparser::ASTNode>> children) {
        return nullptr;
    }
    virtual std::unique_ptr<mcparser::DefStatementASTNode> buildDef(
        std::string identifier,
        mcparser::NodeVisibility visibility,
        std::unique_ptr<mcparser::ASTNode> node) {
        return nullptr;
    }
    virtual std::unique_ptr<mcparser::IntegerASTNode> buildInteger(
        int value) {
        return nullptr;
    }
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_CONTEXT_H_
