#if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_ENUM_HPP)
#define NODE_FFI_GENERATOR_CODE_ENTITY_ENUM_HPP

#include <ffigen/process/code_entity.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <unordered_map>
#include <stdint.h>

namespace ffigen
{
    struct enum_entity : impl::code_entity<enum_entity>
    {
        typedef impl::code_entity<enum_entity> base_type;
        typedef std::unordered_map<std::string, int64_t> values_map_type; // TODO: should use multi index w/ two orders

        enum_entity(std::string const& name, std::string const& file, values_map_type const& values)
            : base_type(name, file)
            , _values(values)
        {}

        values_map_type const& values() const
        {
            return _values;
        }

        std::string get_type_name() const
        {
            return "enum_entity";
        }

    private:
        values_map_type _values;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_ENUM_HPP)