// Copyright 2022 Maniero

#ifndef LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
#define LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
#include <memory>
#include <string>
#include <utility>
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

class Parameter : public ASTNode {
 public:
     std::shared_ptr<std::string> identifier;
     std::shared_ptr<Type> type;
};

class ReferenceIdentifier : public ASTNode {
 public:
     std::shared_ptr<std::string> identifier;
     std::shared_ptr<Type> type;
};

typedef std::vector<std::shared_ptr<Parameter>> Parameters;

class DefStatementASTNode : public ASTNode {
 public:
     std::string identifier;
     NodeVisibility visibility;
     std::shared_ptr<Type> type;
     std::shared_ptr<ASTNode> value;
};

class FunctionStatementASTNode : public ASTNode {
 public:
     std::string identifier;
     NodeVisibility visibility;
     std::shared_ptr<Type> returnType;
     std::shared_ptr<Parameters> parameters;
     std::shared_ptr<ASTNode> body;
};

class FunctionArgument : public ASTNode {
 public:
     std::shared_ptr<ASTNode> value;
};

typedef std::vector<std::shared_ptr<FunctionArgument>> FunctionArguments;

class NativeFunctionCall : public ASTNode {
 public:
     std::shared_ptr<std::string> functionName;
     std::shared_ptr<FunctionArguments> arguments;
     std::shared_ptr<Type> returnType;
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
