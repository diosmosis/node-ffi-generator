#if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_ARRAY_ENTITY_HPP)
#define NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_ARRAY_ENTITY_HPP

#include <ffigen/process/code_entity.hpp>
#include <string>
#include <sstream>

namespace ffigen
{
    struct array_entity : impl::code_entity<array_entity>
    {
        typedef impl::code_entity<array_entity> base_type;

        array_entity(code_entity const& underlying, unsigned int size)
            : base_type("array", "")
            , _underlying(underlying)
        {
            std::stringstream ss;
            ss << "RefArray(" << _underlying.ffi_reference() << ", " << size << ")";
            _ffi_reference = ss.str();
        }

        std::string ffi_reference() const
        {
            return _ffi_reference;
        }

        void fill_dependents() const
        {
            _dependents.push_back(&_underlying);
        }

        std::string get_type_name() const
        {
            return "array_entity";
        }
    private:
        code_entity _underlying;
        std::string _ffi_reference;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_ARRAY_ENTITY_HPP)