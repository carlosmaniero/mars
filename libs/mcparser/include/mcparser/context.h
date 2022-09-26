// Copyright 2022 Maniero

#ifndef LIBS_MCPARSER_INCLUDE_MCPARSER_CONTEXT_H_
#define LIBS_MCPARSER_INCLUDE_MCPARSER_CONTEXT_H_
#include <memory>
#include <string>
#include <vector>

namespace mcparser {
template <class T_AST> class IParserContext {
 public:
    virtual std::unique_ptr<T_AST> buildNamespace(
        std::string namespaceName,
        std::unique_ptr<std::vector<T_AST>> children) {
        return nullptr;
    }
};
}  // namespace mcparser
#endif  // LIBS_MCPARSER_INCLUDE_MCPARSER_CONTEXT_H_
