#include <ffigen/process/code_entity/fundamental_type.hpp>
#include <unordered_map>

namespace ffigen
{
    typedef std::unordered_map<std::string, std::string> ffi_type_map;

    static ffi_type_map clang_type_to_ffi_types;

    static ffi_type_map & get_clang_to_ffi_mapping()
    {
        if (clang_type_to_ffi_types.empty())
        {
            clang_type_to_ffi_types["unsigned char"] = "uchar";
            clang_type_to_ffi_types["char"] = "char";
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
        }

        return clang_type_to_ffi_types;
    }

    fundamental_type_entity::fundamental_type_entity(std::string const& c_type)
        : fundamental_type_entity::base_type(c_type, "")
    {
        ffi_type_map & types = get_clang_to_ffi_mapping();
        ffi_type = "'" + types[c_type] + "'";
    }

    bool fundamental_type_entity::is_supported(std::string const& c_type)
    {
        ffi_type_map & types = get_clang_to_ffi_mapping();
        return types.find(c_type) != types.end();
    }
}