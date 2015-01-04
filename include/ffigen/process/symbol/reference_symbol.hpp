#if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_REFERENCE_HPP)
#define NODE_FFI_GENERATOR_PROCESS_SYMBOL_REFERENCE_HPP

#include <ffigen/process/symbol.hpp>
#include <string>

namespace ffigen
{
    struct reference_symbol : impl::symbol<reference_symbol>
    {
        typedef impl::symbol<reference_symbol> base_type;

        reference_symbol(symbol const& underlying)
            : base_type("pointer", "")
            , _underlying(underlying)
        {}

        std::string ffi_reference() const;

        void fill_dependents() const
        {
            _dependents.push_back(&_underlying);
        }

        std::string get_type_name() const
        {
            return "reference_symbol";
        }

        symbol const& pointee() const
        {
            return _underlying;
        }

    private:
        symbol _underlying;
        mutable std::string _ffi_reference;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_REFERENCE_HPP)