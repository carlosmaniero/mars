// Copyright 2022 Maniero

#include "mcparser/ast.h"

void mcparser::NamespaceASTNode::eval(mcparser::IParserContext* context) {
    spdlog::info("start namespace evaluation");
    context->evalNamespace(this);
}

void mcparser::FunctionStatementASTNode::eval(mcparser::IParserContext* context) {
    spdlog::info("start function evaluation");
    context->evalFunction(this);
}

void mcparser::NativeFunctionCall::eval(mcparser::IParserContext* context) {
    spdlog::info("start native function call evaluation");
    context->evalNativeFunctionCall(this);
}

void mcparser::UserDefinedFunctionCall::eval(mcparser::IParserContext* context) {
    spdlog::info("start user defined function call evaluation");
    context->evalUserDefinedFunctionCall(this);
}

void mcparser::ReferenceIdentifier::eval(mcparser::IParserContext* context) {
    spdlog::info("start reference evaluation");
    context->evalReferenceIdentifier(this);
}

void mcparser::IntegerASTNode::eval(mcparser::IParserContext* context) {
    spdlog::info("start constant integer evaluation");
    context->evalInteger(this);
}
