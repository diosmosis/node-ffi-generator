#if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_REFERENCE_HPP)
#define NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_REFERENCE_HPP

#include <ffigen/process/code_entity.hpp>
#include <string>

namespace ffigen
{
    struct reference_entity : impl::code_entity<reference_entity>
    {
        typedef impl::code_entity<reference_entity> base_type;

        reference_entity(code_entity const& underlying)
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
            return "reference_entity";
        }

        code_entity pointee() const
        {
            return _underlying;
        }

    private:
        code_entity _underlying;
        mutable std::string _ffi_reference;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_REFERENCE_HPP)