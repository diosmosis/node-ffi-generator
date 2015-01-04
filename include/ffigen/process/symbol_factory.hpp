#if !defined(NODE_FFI_GENERATOR_PROCESS_symbol_FACTORY_HPP)
#define NODE_FFI_GENERATOR_PROCESS_symbol_FACTORY_HPP

#include <ffigen/process/symbol_table.hpp>
#include <clang/Basic/SourceManager.h>
#include <clang/AST/Decl.h>

namespace ffigen
{
    struct symbol_factory
    {
        symbol_factory(symbol_table & symbols, clang::SourceManager & source_manager, clang::ASTContext & context)
            : symbols(symbols)
            , source_manager(source_manager)
            , context(context)
        {}

        symbol make(clang::NamedDecl const& node) const;

    private:
        symbol make_enum(clang::EnumDecl const& node, std::string const& name, std::string const& file) const;
        symbol make_function(clang::FunctionDecl const& node, std::string const& name, std::string const& file) const;
        symbol make_record(clang::RecordDecl const& node, std::string const& name, std::string const& file) const;
        symbol make_typedef(clang::TypedefNameDecl const& node, std::string const& name, std::string const& file) const;

        symbol & get_dependent_type(clang::QualType type) const;

        symbol_table & symbols;
        clang::SourceManager & source_manager;
        clang::ASTContext & context;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_symbol_FACTORY_HPP)