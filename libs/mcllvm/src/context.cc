// Copyright 2022 Maniero
#include "mcllvm/context.h"
#include "mcparser/ast.h"
#include "spdlog/spdlog.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/ADT/APInt.h"
#if LLVM_VERSION_MAJOR >= 14
#include "llvm/MC/TargetRegistry.h"
#else
#include "llvm/Support/TargetRegistry.h"
#endif
#include <string>

void mcllvm::LLVMContext::evalNamespace(mcparser::NamespaceASTNode* namespaceAST) {
    spdlog::info("LLVM evaluating namespace: " + namespaceAST->identifier);

    llvmContext = std::make_unique<llvm::LLVMContext>();
    llvmModule = std::make_unique<llvm::Module>(namespaceAST->identifier, *llvmContext);
    llvmBuilder = std::make_unique<llvm::IRBuilder<>>(*llvmContext);

    auto llvmFunction = this->llvmModule->getFunction("printint");
    if (!llvmFunction) {
        auto functionAst = mcparser::FunctionStatementASTNode();
        functionAst.identifier = "printint";
        functionAst.parameters = std::make_shared<mcparser::Parameters>();
        functionAst.parameters->push_back(std::make_shared<mcparser::Parameter>());
        functionAst.parameters->at(0)->identifier = std::make_shared<std::string>("x");
        this->evalPrototype(&functionAst);
    }

    for (auto &node : namespaceAST->nodes) {
        node->eval(this);
    }

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    llvmModule->setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    if (!Target) {
        spdlog::error("LLVM Target Error: " + Error);
        return;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TheTargetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    llvmModule->setDataLayout(TheTargetMachine->createDataLayout());

    auto Filename = "output.o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

    if (EC) {
        spdlog::error("Could not open file: " + EC.message());
        return;
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        spdlog::error("TheTargetMachine can't emit a file of this type");
        return;
    }

    llvmModule->print(llvm::errs(), nullptr);
    pass.run(*llvmModule);
    dest.flush();
}

void mcllvm::LLVMContext::evalPrototype(mcparser::FunctionStatementASTNode* functionAst) {
    spdlog::info("LLVM evaluating function: " + functionAst->identifier);
    std::vector<llvm::Type*> parameters(functionAst->parameters->size());

    spdlog::info("LLVM Creating parameters");
    for (int i = 0; i < functionAst->parameters->size(); i++) {
        parameters.at(i) = llvm::Type::getInt32Ty(*llvmContext);
    }

    // TODO(carlosmaniero): Hard coded int
    spdlog::info("LLVM Defining return type");
    llvm::Type* returnType = llvm::Type::getInt32Ty(*llvmContext);

    llvm::FunctionType* functionType = llvm::FunctionType::get(returnType, parameters, false);

    spdlog::info("LLVM Creating function");
    llvm::Function* function = llvm::Function::Create(
        functionType,
        llvm::Function::ExternalLinkage,
        functionAst->identifier,
        llvmModule.get());

    if (function == NULL) {
        spdlog::error("LLVM could not create a function");
        return;
    }
}

void mcllvm::LLVMContext::evalFunction(mcparser::FunctionStatementASTNode* functionAst) {
    this->evalPrototype(functionAst);
    auto function = this->llvmModule->getFunction(functionAst->identifier);

    spdlog::info("LLVM naming parameters");
    unsigned Idx = 0;
    for (auto &functionArg : function->args()) {
        functionArg.setName(*functionAst->parameters->at(Idx++)->identifier);
    }

    spdlog::info("LLVM creating block");
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*llvmContext, "entry", function);
    llvmBuilder->SetInsertPoint(BB);

    scopeValues.clear();
    for (auto &arg : function->args()) {
        llvm::IRBuilder<> tempBuilder(
            &function->getEntryBlock(),
            function->getEntryBlock().begin());

        // Create an alloca for this variable.
        llvm::AllocaInst* parameterAllocation = tempBuilder.CreateAlloca(
            llvm::Type::getInt32Ty(*llvmContext), nullptr, arg.getName());

        // Store the initial value into the alloca.
        llvmBuilder->CreateStore(&arg, parameterAllocation);

        // Add arguments to variable symbol table.
        scopeValues[std::string(arg.getName())] = parameterAllocation;
    }

    spdlog::info("LLVM eval function body");
    functionAst->body->eval(this);

    llvmBuilder->CreateRet(this->latestEvaluatedValue);

    if (llvm::verifyFunction(*function)) {
        spdlog::error("Error during function code gen: " + functionAst->identifier);
        llvmModule->print(llvm::errs(), nullptr);
    }
}

void mcllvm::LLVMContext::evalUserDefinedFunctionCall(mcparser::UserDefinedFunctionCall* functionAst) {
    spdlog::info("LLVM evaluating user defined function call of: " + *functionAst->functionName);
    llvm::Function* llvmFunction = llvmModule->getFunction(*functionAst->functionName);

    std::vector<llvm::Value*> llvmFunctionArguments;
    for (unsigned i = 0, e = functionAst->arguments->size(); i != e; ++i) {
        this->evalValueFrom(functionAst->arguments->at(i)->value.get());
        llvmFunctionArguments.push_back(this->latestEvaluatedValue);
    }

    this->latestEvaluatedValue = llvmBuilder->CreateCall(llvmFunction, llvmFunctionArguments);
}

void mcllvm::LLVMContext::evalNativeFunctionCall(mcparser::NativeFunctionCall* functionAst) {
    spdlog::info("LLVM evaluating native function call of: " + *functionAst->functionName);

    if ("printint" == *functionAst->functionName) {
        auto llvmFunction = this->llvmModule->getFunction("printint");

        if (llvmFunction == nullptr) {
            spdlog::error("LLVM could not find printint function");
            return;
        }

        this->evalValueFrom(functionAst->arguments->at(0)->value.get());
        llvm::Value* first = this->latestEvaluatedValue;
        std::vector<llvm::Value*> llvmFunctionArguments;
        llvmFunctionArguments.push_back(first);

        this->latestEvaluatedValue = llvmBuilder->CreateCall(llvmFunction, llvmFunctionArguments);
        return;
    }

    if ("+" != *functionAst->functionName) {
        spdlog::error("Native function not supported: " + *functionAst->functionName);
        return;
    }

    if (functionAst->arguments->size() != 2) {
        spdlog::error("The + function requires only 2 arguments" + *functionAst->functionName);
        return;
    }

    spdlog::info("LLVM evaluating first function arguments");

    this->evalValueFrom(functionAst->arguments->at(0)->value.get());
    llvm::Value* first = this->latestEvaluatedValue;

    spdlog::info("LLVM evaluating second function arguments");
    this->evalValueFrom(functionAst->arguments->at(1)->value.get());
    llvm::Value* second = this->latestEvaluatedValue;

    this->latestEvaluatedValue = llvmBuilder->CreateAdd(first, second, "addtmp");
}

void mcllvm::LLVMContext::evalValueFrom(mcparser::ASTNode* node) {
    latestEvaluatedValue = NULL;
    node->eval(this);
}

void mcllvm::LLVMContext::evalReferenceIdentifier(mcparser::ReferenceIdentifier* reference) {
    spdlog::info("LLVM evaluating reference identifier from scope: " + *reference->identifier);
    auto myReference = scopeValues.at(*reference->identifier);

    this->latestEvaluatedValue = llvmBuilder->CreateLoad(
        llvm::Type::getInt32Ty(*llvmContext), myReference, *reference->identifier);
}

void mcllvm::LLVMContext::evalInteger(mcparser::IntegerASTNode* integer) {
    spdlog::info("LLVM evaluating constant integer: " + std::to_string(integer->value));
    llvm::Type *i32_type = llvm::IntegerType::getInt32Ty(*llvmContext);

    this->latestEvaluatedValue = llvm::ConstantInt::get(i32_type, integer->value, true);
}
