#if !defined(NODE_FFI_GENERATOR_SYMBOL_FUNCTION_HPP)
#define NODE_FFI_GENERATOR_SYMBOL_FUNCTION_HPP

#include <ffigen/process/symbol.hpp>
#include <ffigen/generate/generator.hpp>
#include <list>
#include <functional>
#include <iosfwd>

namespace ffigen
{
    struct function_symbol : impl::symbol<function_symbol>
    {
        struct generator : impl::generator_base
        {
            generator(generator_factory const& factory, function_symbol const& entity, unsigned int indent = 0)
                : impl::generator_base(factory, indent)
                , entity(entity)
            {}

            void operator()(std::ostream & os) const;

            function_symbol const& entity;
        };

        typedef impl::symbol<function_symbol> base_type;
        typedef std::list<symbol> arguments_list_type;

        function_symbol(std::string const& name
          , std::string const& file
          , symbol const& return_type
          , arguments_list_type const& arguments
          , bool is_variadic
        );

        symbol const& return_type() const
        {
            return _return_type;
        }

        arguments_list_type const& argument_types() const
        {
            return _arguments;
        }

        void fill_dependents() const;

        std::string get_type_name() const
        {
            return "function_symbol";
        }

        bool is_variadic() const
        {
            return _is_variadic;
        }

    private:
        symbol _return_type;
        arguments_list_type _arguments;
        bool _is_variadic;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_SYMBOL_FUNCTION_HPP)
