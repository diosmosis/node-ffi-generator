#if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_FUNCTION_HPP)
#define NODE_FFI_GENERATOR_CODE_ENTITY_FUNCTION_HPP

#include <ffigen/process/code_entity.hpp>
#include <list>
#include <functional>
#include <iostream>
namespace ffigen
{
    struct function : impl::code_entity<function>
    {
        typedef impl::code_entity<function> base_type;
        typedef std::list<code_entity> arguments_list_type;

        function(std::string const& name
          , std::string const& file
          , code_entity const& return_type
          , arguments_list_type const& arguments
        ) : base_type(name, file)
          , _return_type(return_type)
          , _arguments(arguments)
        {}

        code_entity const& return_type() const
        {
            return _return_type;
        }

        arguments_list_type const& argument_types() const
        {
            return _arguments;
        }

        void fill_dependents() const
        {
            _dependents.push_back(&_return_type);
            for (auto const& param_type : _arguments)
            {
                _dependents.push_back(&param_type);
            }
        }

        std::string get_type_name() const
        {
            return "function_entity";
        }

    private:
        code_entity _return_type;
        arguments_list_type _arguments;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_FUNCTION_HPP)
