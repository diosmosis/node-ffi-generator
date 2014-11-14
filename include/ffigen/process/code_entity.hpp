#if !defined(NODE_FFIGEN_PROCESS_CODE_ENTITY_HPP)
#define NODE_FFIGEN_PROCESS_CODE_ENTITY_HPP

#include <ffigen/utility/type_erasure_base.hpp>
#include <clang/AST/Decl.h>
#include <memory>
#include <list>
#include <stdexcept>

namespace ffigen
{
    struct symbol_table;
    struct code_entity;

    namespace impl
    {
        struct code_entity_base : utility::type_erasure_base
        {
            typedef utility::type_erasure_base base_type;
            typedef std::list<code_entity const*> dependents_container_type;

            code_entity_base(unsigned int type_id, std::string const& name, std::string const& file)
                : base_type(type_id)
                , _name(name)
                , _file(file)
            {}

            virtual ~code_entity_base() {}

            std::string const& name() const
            {
                return _name;
            }

            std::string const& file() const
            {
                return _file;
            }

            dependents_container_type const& dependents() const
            {
                return _dependents;
            }

            virtual std::string ffi_reference() const
            {
                return std::string("_library.") + _name;
            }

            virtual bool is_anonymous() const
            {
                return false;
            }

        protected:
            std::string _name;
            std::string _file;
            dependents_container_type _dependents;
        };

        template <typename Derived>
        using code_entity = utility::type_erasure_impl<Derived, code_entity_base>;
    }

    struct code_entity
    {
        typedef impl::code_entity_base::dependents_container_type dependents_container_type;

        code_entity()
            : impl()
        {}

        template <typename T>
        code_entity(T const& entity)
            : impl(new T(entity))
        {}

        code_entity(code_entity const& entity)
            : impl(entity.impl)
        {}

        code_entity & operator = (code_entity const& entity)
        {
            impl = entity.impl;
            return *this;
        }

        std::string name() const
        {
            check_impl();

            return impl->name();
        }

        std::string file() const
        {
            check_impl();

            return impl->file();
        }

        dependents_container_type const& dependents() const
        {
            check_impl();

            return impl->dependents();
        }

        std::string ffi_reference() const
        {
            check_impl();

            return impl->ffi_reference();
        }

        bool is_anonymous() const
        {
            check_impl();

            return impl->is_anonymous();
        }

        template <typename T>
        bool is_a() const
        {
            return impl && impl->is_a<T>();
        }

        template <typename T>
        T & cast()
        {
            check_impl();

            return static_cast<T &>(*impl);
        }

        template <typename T>
        T const& cast() const
        {
            check_impl();

            return static_cast<T const&>(*impl);
        }

        explicit operator bool() const
        {
            return impl.get();
        }

        void * get_impl() const
        {
            return impl.get();
        }

    private:
        void check_impl() const
        {
            if (!*this)
            {
                throw std::runtime_error("Null code entity being accessed!");
            }
        }

        std::shared_ptr<impl::code_entity_base> impl;
    };
}

#endif // #if !defined(NODE_FFIGEN_PROCESS_CODE_ENTITY_HPP
