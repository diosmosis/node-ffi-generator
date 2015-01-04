#if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_FUNCTION_HPP)
#define NODE_FFI_GENERATOR_CODE_ENTITY_FUNCTION_HPP

#include <ffigen/process/code_entity.hpp>
#include <ffigen/generate/generator.hpp>
#include <list>
#include <functional>
#include <iosfwd>

namespace ffigen
{
    struct function : impl::code_entity<function>
    {
        struct generator : impl::generator_base
        {
            generator(generator_factory const& factory, function const& entity, unsigned int indent = 0)
                : impl::generator_base(factory, indent)
                , entity(entity)
            {}

            void operator()(std::ostream & os) const;

            function const& entity;
        };

        typedef impl::code_entity<function> base_type;
        typedef std::list<code_entity> arguments_list_type;

        function(std::string const& name
          , std::string const& file
          , code_entity const& return_type
          , arguments_list_type const& arguments
          , bool is_variadic
        );

        code_entity const& return_type() const
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
            return "function_entity";
        }

        bool is_variadic() const
        {
            return _is_variadic;
        }

    private:
        code_entity _return_type;
        arguments_list_type _arguments;
        bool _is_variadic;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_FUNCTION_HPP)
