#if !defined(NODE_FFI_GENERATOR_FFIGEN_PROCESS_FILTERS_INCLUDE_SYMBOLS_IN_LIB_HPP)
#define NODE_FFI_GENERATOR_FFIGEN_PROCESS_FILTERS_INCLUDE_SYMBOLS_IN_LIB_HPP

#include <annex/annex.hpp>
#include <clang/AST/Decl.h>
#include <string>
#include <memory>

namespace ffigen
{
    struct code_entity;

    struct include_symbols_in_lib
    {
        typedef bool result_type;

        include_symbols_in_lib(std::string const& path);

        bool operator()(clang::NamedDecl const& node) const;

        std::shared_ptr<annex::shared_library> lib;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_FFIGEN_PROCESS_FILTERS_INCLUDE_SYMBOLS_IN_LIB_HPP)