#if !defined(NODE_FFI_GENERATOR_SYMBOL_ENUM_HPP)
#define NODE_FFI_GENERATOR_SYMBOL_ENUM_HPP

#include <ffigen/process/symbol.hpp>
#include <ffigen/generate/generator.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <unordered_map>
#include <stdint.h>
#include <iosfwd>

namespace ffigen
{
    struct enum_symbol : impl::symbol<enum_symbol>
    {
        struct generator : impl::generator_base
        {
            generator(generator_factory const& factory, enum_symbol const& entity, unsigned int indent = 0)
                : impl::generator_base(factory, indent)
                , entity(entity)
            {}

            void operator()(std::ostream & os) const;

            enum_symbol const& entity;
        };

        typedef impl::symbol<enum_symbol> base_type;
        typedef std::unordered_map<std::string, int64_t> values_map_type; // TODO: should use multi index w/ two orders

        enum_symbol(std::string const& name, std::string const& file, values_map_type const& values);

        values_map_type const& values() const
        {
            return _values;
        }

        std::string get_type_name() const
        {
            return "enum_symbol";
        }

    private:
        values_map_type _values;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_SYMBOL_ENUM_HPP)