#if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_TYPEDEF_HPP)
#define NODE_FFI_GENERATOR_CODE_ENTITY_TYPEDEF_HPP

#include <ffigen/process/code_entity.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>

namespace ffigen
{
	struct typedef_entity : impl::code_entity<typedef_entity>
	{
		typedef impl::code_entity<typedef_entity> base_type;

		typedef_entity(std::string const& name, std::string const& file, code_entity const& type)
			: base_type(name, file)
			, _type(type)
		{
			_dependents.push_back(&type);
		}

		code_entity const& alias_type() const
		{
			return _type;
		}

	private:
		code_entity const& _type;
	};
}

#endif // #if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_TYPEDEF_HPP)