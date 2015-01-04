#include <ffigen/process/symbol/fundamental_type_symbol.hpp>
#include <ffigen/utility/logger.hpp>
#include <unordered_map>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    typedef std::unordered_map<std::string, std::string> ffi_type_map;

    static ffi_type_map clang_type_to_ffi_types;

    static ffi_type_map & get_clang_to_ffi_mapping()
    {
        if (clang_type_to_ffi_types.empty())
        {
            clang_type_to_ffi_types["unsigned char"] = "uchar";
            clang_type_to_ffi_types["char"] = "char";
            clang_type_to_ffi_types["signed char"] = "char";
            clang_type_to_ffi_types["short"] = "short";
            clang_type_to_ffi_types["unsigned short"] = "ushort";
            clang_type_to_ffi_types["int"] = "int";
            clang_type_to_ffi_types["unsigned int"] = "uint";
            clang_type_to_ffi_types["long"] = "long";
            clang_type_to_ffi_types["unsigned long"] = "ulong";
            clang_type_to_ffi_types["long long"] = "longlong";
            clang_type_to_ffi_types["unsigned long long"] = "ulonglong";
            clang_type_to_ffi_types["size_t"] = "size_t";
            clang_type_to_ffi_types["ptrdiff_t"] = "size_t";

            // TODO: ref.js doesn't have a longdouble type, though node-ffi seems to
            clang_type_to_ffi_types["long double"] = "double";

            clang_type_to_ffi_types["int8_t"] = "int8";
            clang_type_to_ffi_types["uint8_t"] = "uint8";
            clang_type_to_ffi_types["int16_t"] = "int16";
            clang_type_to_ffi_types["uint16_t"] = "uint16";
            clang_type_to_ffi_types["int32_t"] = "int32";
            clang_type_to_ffi_types["uint32_t"] = "uint32";
            clang_type_to_ffi_types["int64_t"] = "int64";
            clang_type_to_ffi_types["uint64_t"] = "uint64";
            clang_type_to_ffi_types["float"] = "float";
            clang_type_to_ffi_types["double"] = "double";

            clang_type_to_ffi_types["void"] = "void";

            // whitelist variadic arg types to void TODO: bit of a hack, not sure how to handle this case
            // in generic manner
            clang_type_to_ffi_types["va_list"] = "void";
            clang_type_to_ffi_types["__builtin_va_list"] = "void";
            clang_type_to_ffi_types["__gnuc_va_list"] = "void";
        }

        return clang_type_to_ffi_types;
    }

    fundamental_type_symbol::fundamental_type_symbol(std::string const& c_type)
        : fundamental_type_symbol::base_type(c_type, "")
    {
        ffi_type_map & types = get_clang_to_ffi_mapping();
        ffi_type = "'" + types[c_type] + "'";
    }

    bool fundamental_type_symbol::is_supported(std::string const& c_type)
    {
        ffi_type_map & types = get_clang_to_ffi_mapping();
        return types.find(c_type) != types.end();
    }

    fundamental_type_symbol fundamental_type_symbol::make_int_from_size(int64_t size)
    {
        switch (size)
        {
        case 1:
            return fundamental_type_symbol("int8_t");
        case 2:
            return fundamental_type_symbol("int16_t");
        case 4:
            return fundamental_type_symbol("int32_t");
        case 8:
            return fundamental_type_symbol("int64_t");
        default:
            warning() << "WARNING: Unknown integer size '" << size << "' found in fundamental_type_symbol::make_int_from_size()."
                      << std::endl;

            return fundamental_type_symbol("int");
        }
    }
}