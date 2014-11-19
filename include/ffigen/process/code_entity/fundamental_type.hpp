#if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_FUNDAMENTAL_TYPE_HPP)
#define NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_FUNDAMENTAL_TYPE_HPP

#include <ffigen/process/code_entity.hpp>
#include <string>

namespace ffigen
{
    struct fundamental_type_entity : impl::code_entity<fundamental_type_entity>
    {
        typedef impl::code_entity<fundamental_type_entity> base_type;

        fundamental_type_entity(std::string const& c_type);

        static bool is_supported(std::string const& c_type);

        std::string ffi_reference() const
        {
            return ffi_type;
        }

        std::string get_type_name() const
        {
            return "fundamental_type_entity";
        }
    private:
        std::string ffi_type;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_FUNDAMENTAL_TYPE_HPP)