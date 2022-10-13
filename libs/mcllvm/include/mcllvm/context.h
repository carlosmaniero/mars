// Copyright 2022 Maniero
#ifndef LIBS_MCLLVM_INCLUDE_MCLLVM_CONTEXT_H_
#define LIBS_MCLLVM_INCLUDE_MCLLVM_CONTEXT_H_
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "mcparser/ast.h"
#include "spdlog/spdlog.h"

namespace mcllvm {
class LLVMContext : public mcparser::IParserContext {
 public:
    std::unique_ptr<llvm::LLVMContext> llvmContext = nullptr;
    std::unique_ptr<llvm::Module> llvmModule = nullptr;
    std::unique_ptr<llvm::IRBuilder<>> llvmBuilder = nullptr;
    std::map<std::string, llvm::AllocaInst*> scopeValues;

    virtual void evalNamespace(mcparser::NamespaceASTNode* namespaceAST);
    virtual void evalFunction(mcparser::FunctionStatementASTNode* functionAst);
    virtual void evalNativeFunctionCall(mcparser::NativeFunctionCall* functionAst);
    virtual void evalReferenceIdentifier(mcparser::ReferenceIdentifier* reference);
 private:
    llvm::Value* latestEvaluatedValue = NULL;
    void evalValueFrom(mcparser::ASTNode* node);
};
}  // namespace mcllvm
#endif  // LIBS_MCLLVM_INCLUDE_MCLLVM_CONTEXT_H_
