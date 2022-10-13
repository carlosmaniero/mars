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
    spdlog::info("start function call evaluation");
    context->evalNativeFunctionCall(this);
}

void mcparser::ReferenceIdentifier::eval(mcparser::IParserContext* context) {
    spdlog::info("start reference evaluation");
    context->evalReferenceIdentifier(this);
}
