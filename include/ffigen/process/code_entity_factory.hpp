#if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_FACTORY_HPP)
#define NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_FACTORY_HPP

#include <ffigen/process/symbol_table.hpp>
#include <clang/Basic/SourceManager.h>
#include <clang/AST/Decl.h>

namespace ffigen
{
	struct code_entity_factory
	{
		code_entity_factory(symbol_table & symbols, clang::SourceManager & source_manager)
			: symbols(symbols)
			, source_manager(source_manager)
		{}

		code_entity make(clang::NamedDecl const& node) const;

	private:
		code_entity make_enum(clang::EnumDecl const& node, std::string const& name, std::string const& file) const;
		code_entity make_function(clang::FunctionDecl const& node, std::string const& name, std::string const& file) const;
		code_entity make_struct(clang::RecordDecl const& node, std::string const& name, std::string const& file) const;
		code_entity make_typedef(clang::TypedefNameDecl const& node, std::string const& name, std::string const& file) const;
		code_entity make_union(clang::RecordDecl const& node, std::string const& name, std::string const& file) const;

		code_entity const& get_dependent_type(clang::QualType type) const;

		symbol_table & symbols;
		clang::SourceManager & source_manager;
	};
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_FACTORY_HPP)