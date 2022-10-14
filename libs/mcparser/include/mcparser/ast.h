// Copyright 2022 Maniero

#ifndef LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
#define LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "spdlog/spdlog.h"

namespace mcparser {

class IParserContext;

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
  virtual void eval(mcparser::IParserContext* context) {
    spdlog::warn("AST with undefined evaluation.");
  }
};

class NamespaceASTNode : public ASTNode {
 public:
     std::string identifier;
     std::vector<std::shared_ptr<ASTNode>> nodes;

     virtual void eval(mcparser::IParserContext* context);
};

class IntegerASTNode : public ASTNode {
 public:
     int value;

     virtual void eval(mcparser::IParserContext* context);
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

     virtual void eval(mcparser::IParserContext* context);
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

     virtual void eval(mcparser::IParserContext* context);
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

     virtual void eval(mcparser::IParserContext* context);
};

class IParserContext {
 public:
    virtual void evalNamespace(mcparser::NamespaceASTNode* namespaceAST) {
    }
    virtual void evalFunction(mcparser::FunctionStatementASTNode* functionAST) {
    }
    virtual void evalNativeFunctionCall(mcparser::NativeFunctionCall* functionAst) {
    }
    virtual void evalReferenceIdentifier(mcparser::ReferenceIdentifier* reference) {
    }
    virtual void evalInteger(mcparser::IntegerASTNode* reference) {
    }
    virtual ~IParserContext() = default;
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_AST_H_
