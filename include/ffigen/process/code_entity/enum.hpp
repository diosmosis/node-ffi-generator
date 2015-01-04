#if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_ENUM_HPP)
#define NODE_FFI_GENERATOR_CODE_ENTITY_ENUM_HPP

#include <ffigen/process/code_entity.hpp>
#include <ffigen/generate/generator.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <unordered_map>
#include <stdint.h>
#include <iosfwd>

namespace ffigen
{
    struct enum_entity : impl::code_entity<enum_entity>
    {
        struct generator : impl::generator_base
        {
            generator(generator_factory const& factory, enum_entity const& entity, unsigned int indent = 0)
                : impl::generator_base(factory, indent)
                , entity(entity)
            {}

            void operator()(std::ostream & os) const;

            enum_entity const& entity;
        };

        typedef impl::code_entity<enum_entity> base_type;
        typedef std::unordered_map<std::string, int64_t> values_map_type; // TODO: should use multi index w/ two orders

        enum_entity(std::string const& name, std::string const& file, values_map_type const& values);

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