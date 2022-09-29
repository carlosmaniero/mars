// Copyright 2022 Maniero

#ifndef LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
#define LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
#include <memory>
#include <string>
#include <vector>

namespace mcparser {
enum NodeVisibility {
  node_visibility_public,
  node_visibility_private
};

class ASTNode {};

class NamespaceASTNode : public ASTNode {
 public:
     std::string identifier;
     std::vector<std::unique_ptr<ASTNode>> nodes;
};

class IntegerASTNode : public ASTNode {
 public:
     int value;
};

class DefStatementASTNode : public ASTNode {
 public:
     std::string identifier;
     NodeVisibility visibility;
     std::unique_ptr<ASTNode> value;
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
