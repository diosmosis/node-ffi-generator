#if !defined(NODE_FFI_GENERATOR_SYMBOL_FUNCTION_POINTER_ENTITY_HPP)
#define NODE_FFI_GENERATOR_SYMBOL_FUNCTION_POINTER_ENTITY_HPP

#include <ffigen/process/symbol.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <map>

namespace ffigen
{
    struct function_pointer_symbol : impl::symbol<function_pointer_symbol>
    {
        typedef impl::symbol<function_pointer_symbol> base_type;
        typedef std::list<symbol> arguments_list_type;

        function_pointer_symbol(std::string const& type_str, symbol const& return_type, arguments_list_type const& arguments)
            : base_type(type_str, "")
            , _return_type(return_type)
            , _arguments(arguments)
        {}

        std::string ffi_reference() const;

        std::string get_type_name() const
        {
            return "function_pointer_symbol";
        }

        void fill_dependents() const
        {
            _dependents.push_back(&_return_type);
            for (auto const& param_type : _arguments) {
                _dependents.push_back(&param_type);
            }
        }

    private:
        void set_ffi_reference(clang::QualType const& function_type);

        symbol _return_type;
        arguments_list_type _arguments;
        mutable std::string _ffi_reference;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_SYMBOL_FUNCTION_POINTER_ENTITY_HPP)
