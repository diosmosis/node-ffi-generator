#if !defined(NODE_FFI_GENERATOR_SYMBOL_TYPEDEF_HPP)
#define NODE_FFI_GENERATOR_SYMBOL_TYPEDEF_HPP

#include <ffigen/process/symbol.hpp>
#include <ffigen/generate/generator.hpp>
#include <iosfwd>

namespace ffigen
{
    struct typedef_symbol : impl::symbol<typedef_symbol>
    {
        struct generator : impl::generator_base
        {
            generator(generator_factory const& factory, typedef_symbol const& entity, unsigned int indent = 0)
                : impl::generator_base(factory, indent)
                , entity(entity)
            {}

            void operator()(std::ostream & os) const;

            typedef_symbol const& entity;
        };

        typedef impl::symbol<typedef_symbol> base_type;

        typedef_symbol(std::string const& name, std::string const& file, symbol const& type)
            : base_type(name, file)
            , _type(type)
        {}

        symbol const& alias_type() const
        {
            return _type;
        }

        void fill_dependents() const
        {
            _dependents.push_back(&_type);
        }

        std::string get_type_name() const
        {
            return "typedef_symbol";
        }
    private:
        symbol _type;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_SYMBOL_TYPEDEF_HPP)