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

class Type {
 public:
  virtual ~Type() = default;
};

class NativeIntegerType : public Type {
};

class ASTNode {
 public:
  virtual ~ASTNode() = default;
};

class NamespaceASTNode : public ASTNode {
 public:
     std::string identifier;
     std::vector<std::shared_ptr<ASTNode>> nodes;
};

class IntegerASTNode : public ASTNode {
 public:
     int value;
};

class DefStatementASTNode : public ASTNode {
 public:
     std::string identifier;
     NodeVisibility visibility;
     std::shared_ptr<Type> type;
     std::shared_ptr<ASTNode> value;
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
