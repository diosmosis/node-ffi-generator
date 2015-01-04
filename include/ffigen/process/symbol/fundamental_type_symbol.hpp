#if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_FUNDAMENTAL_TYPE_HPP)
#define NODE_FFI_GENERATOR_PROCESS_SYMBOL_FUNDAMENTAL_TYPE_HPP

#include <ffigen/process/symbol.hpp>
#include <string>

namespace ffigen
{
    struct fundamental_type_symbol : impl::symbol<fundamental_type_symbol>
    {
        typedef impl::symbol<fundamental_type_symbol> base_type;

        fundamental_type_symbol(std::string const& c_type);

        static bool is_supported(std::string const& c_type);
        static fundamental_type_symbol make_int_from_size(int64_t size);

        std::string ffi_reference() const
        {
            return ffi_type;
        }

        std::string get_type_name() const
        {
            return "fundamental_type_symbol";
        }
    private:
        std::string ffi_type;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_FUNDAMENTAL_TYPE_HPP)