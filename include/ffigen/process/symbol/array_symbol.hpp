#if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_ARRAY_ENTITY_HPP)
#define NODE_FFI_GENERATOR_PROCESS_SYMBOL_ARRAY_ENTITY_HPP

#include <ffigen/process/symbol.hpp>
#include <string>
#include <sstream>

namespace ffigen
{
    struct array_symbol : impl::symbol<array_symbol>
    {
        typedef impl::symbol<array_symbol> base_type;

        array_symbol(symbol const& underlying, unsigned int size)
            : base_type("array", "")
            , _underlying(underlying)
            , _size(size)
        {}

        std::string ffi_reference() const;

        void fill_dependents() const
        {
            _dependents.push_back(&_underlying);
        }

        std::string get_type_name() const
        {
            return "array_symbol";
        }

        symbol const& element_type() const
        {
            return _underlying;
        }

    private:
        symbol _underlying;
        unsigned int _size;
        mutable std::string _ffi_reference;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_ARRAY_ENTITY_HPP)