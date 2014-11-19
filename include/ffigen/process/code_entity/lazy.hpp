#if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_LAZY_HPP)
#define NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_LAZY_HPP

#include <ffigen/process/code_entity.hpp>
#include <ffigen/process/symbol_table.hpp>

namespace ffigen
{
    struct lazy_code_entity : impl::code_entity<lazy_code_entity>
    {
        typedef impl::code_entity<lazy_code_entity> base_type;

        lazy_code_entity(std::string const& fqn, symbol_table & symbols)
            : base_type("", "")
            , fqn(fqn)
            , impl(fqn)
            , symbols(symbols)
        {}

        std::string name() const
        {
            set_internal_pointer();

            return impl.name();
        }

        std::string file() const
        {
            set_internal_pointer();

            return impl.file();
        }

        dependents_container_type const& dependents() const
        {
            set_internal_pointer();

            return impl.dependents();
        }

        std::string ffi_reference() const
        {
            set_internal_pointer();

            return impl.ffi_reference();
        }

        bool is_anonymous() const
        {
            set_internal_pointer();

            return impl.is_anonymous();
        }

        bool is_valid() const
        {
            return static_cast<bool>(impl);
        }

        std::string get_type_name() const
        {
            set_internal_pointer();
            
            return impl.get_type_name();
        }

        code_entity const& get_impl() const
        {
            return impl;
        }

    private:
        void set_internal_pointer() const;

        std::string fqn;
        code_entity impl;
        symbol_table & symbols;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_LAZY_HPP)