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
            check_internal_pointer();

            return impl.name();
        }

        std::string file() const
        {
            set_internal_pointer();
            check_internal_pointer();

            return impl.file();
        }

        dependents_container_type const& dependents() const
        {
            set_internal_pointer();
            check_internal_pointer();

            return impl.dependents();
        }

        std::string ffi_reference() const
        {
            set_internal_pointer();
            check_internal_pointer();

            return impl.ffi_reference();
        }

        bool is_anonymous() const
        {
            set_internal_pointer();
            check_internal_pointer();

            return impl.is_anonymous();
        }

        bool is_valid() const
        {
            set_internal_pointer();
            check_internal_pointer();

            return static_cast<bool>(impl);
        }

        std::string get_type_name() const
        {
            set_internal_pointer();
            check_internal_pointer();
            
            return impl.get_type_name();
        }

        code_entity const& get_impl() const
        {
            set_internal_pointer();
            check_internal_pointer();

            return impl;
        }

        // TODO: need to enforce that lazy code entity cannot reference lazy code entity. otherwise
        //       hard to debug errors may occur.
        bool is_equal(impl::code_entity_base const* other) const
        {
            set_internal_pointer();
            check_internal_pointer();

            if (this->is_same_class_as(other)) {
                other = static_cast<lazy_code_entity const*>(other)->get_impl().get_impl();
            }

            // NOTE: if impl is null, it is unresolved and not equal to any other symbol
            return impl ? impl.get_impl()->is_equal(other) : false;
        }

        std::string const& accessed_name() const
        {
            return fqn;
        }

    private:
        void set_internal_pointer() const;
        void check_internal_pointer() const;

        std::string fqn;
        code_entity impl;
        symbol_table & symbols;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_LAZY_HPP)