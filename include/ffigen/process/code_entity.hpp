#if !defined(NODE_FFIGEN_PROCESS_CODE_ENTITY_HPP)
#define NODE_FFIGEN_PROCESS_CODE_ENTITY_HPP

#include <ffigen/utility/type_erasure_base.hpp>
#include <ffigen/utility/exceptions.hpp>
#include <ffigen/utility/error_codes.hpp>
#include <memory>
#include <list>
#include <stdexcept>

namespace ffigen
{
    struct symbol_table;
    struct code_entity;
    struct lazy_code_entity;

    namespace impl
    {
        struct code_entity_base : utility::type_erasure_base
        {
            typedef utility::type_erasure_base base_type;
            typedef std::list<code_entity const*> dependents_container_type;

            code_entity_base(unsigned int type_id, std::string const& name, std::string const& file);

            virtual ~code_entity_base() {}

            virtual std::string name() const
            {
                return _name;
            }

            virtual std::string file() const
            {
                return _file;
            }

            virtual dependents_container_type const& dependents() const
            {
                if (_dependents.empty()) {
                    fill_dependents();
                }
                return _dependents;
            }

            virtual std::string ffi_reference() const
            {
                if (is_anonymous()) {
                    throw fatal_error("Attempting to get ffi reference to anonymous symbol.", error_codes::UNEXPECTED);
                }

                return std::string("_library.") + _name;
            }

            virtual bool is_anonymous() const
            {
                return false;
            }

            virtual bool is_valid() const
            {
                return true;
            }

            virtual bool is_equal(code_entity_base const* other) const;

            virtual void fill_dependents() const {}

            virtual std::string get_type_name() const = 0;

        protected:
            std::string _name;
            std::string _file;
            mutable dependents_container_type _dependents;
        };

        template <typename Derived>
        using code_entity = utility::type_erasure_impl<Derived, code_entity_base>;
    }

    struct code_entity
    {
        typedef impl::code_entity_base::dependents_container_type dependents_container_type;

        code_entity(std::string const& accessed_name = "")
            : impl()
            , _accessed_name(accessed_name)
        {}

        template <typename T>
        code_entity(T const& entity)
            : impl(new T(entity))
        {}

        code_entity(code_entity const& entity)
            : impl(entity.impl)
            , _accessed_name(entity._accessed_name)
        {}

        code_entity & operator = (code_entity const& entity)
        {
            impl = entity.impl;
            _accessed_name = entity._accessed_name;
            return *this;
        }

        std::string name() const
        {
            check_impl("name");

            return impl->name();
        }

        std::string file() const
        {
            check_impl("file");

            return impl->file();
        }

        dependents_container_type const& dependents() const
        {
            check_impl("dependents");

            return impl->dependents();
        }

        std::string ffi_reference() const
        {
            check_impl("ffi_reference");

            return impl->ffi_reference();
        }

        bool is_anonymous() const
        {
            check_impl("is_anonymous");

            return impl->is_anonymous();
        }

        template <typename T>
        bool is_a() const
        {
            check_not_lazy<T>();

            return impl && impl->is_a<T>();
        }

        template <typename T>
        T & cast()
        {
            check_not_lazy<T>();
            check_impl("cast");

            return static_cast<T &>(*impl);
        }

        template <typename T>
        T const& cast() const
        {
            check_impl("cast const");

            return static_cast<T const&>(*impl);
        }

        explicit operator bool() const
        {
            return impl.get() && impl->is_valid();
        }

        impl::code_entity_base * get_impl() const
        {
            return impl.get();
        }

        std::string get_type_name() const
        {
            return impl->get_type_name();
        }

        bool operator == (code_entity const& other) const
        {
            if (!impl || !other.impl) {
                return impl == other.impl;
            } else {
                return impl->is_equal(other.get_impl());
            }
        }

        std::string const& accessed_name() const
        {
            return _accessed_name;
        }

    protected:
        void check_impl(char const* function) const
        {
            if (!impl)
            {
                throw std::runtime_error(std::string("Null code entity '") + _accessed_name + "' being accessed in '"
                    + std::string(function) + "'!");
            }
        }

        template <typename T>
        void check_not_lazy() const
        {
            //static_assert(!std::is_same<T, lazy_code_entity>::value,
            //    "should not have to check for lazy_code_entity, symbol_table should resolve these types after parsing");
        }

        std::shared_ptr<impl::code_entity_base> impl;

        std::string _accessed_name;
    };
}

namespace std
{
    template <>
    struct hash<ffigen::code_entity>
        : public hash<std::string>
    {
        typedef hash<std::string> base_type;

        size_t operator()(ffigen::code_entity const& x) const;
    };
}
#endif // #if !defined(NODE_FFIGEN_PROCESS_CODE_ENTITY_HPP
