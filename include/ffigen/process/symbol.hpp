#if !defined(NODE_FFIGEN_PROCESS_SYMBOL_HPP)
#define NODE_FFIGEN_PROCESS_SYMBOL_HPP

#include <ffigen/utility/type_erasure_base.hpp>
#include <ffigen/utility/exceptions.hpp>
#include <ffigen/utility/error_codes.hpp>
#include <memory>
#include <list>
#include <stdexcept>

namespace ffigen
{
    struct symbol_table;
    struct symbol;
    struct lazy_symbol;

    namespace impl
    {
        struct symbol_base : utility::type_erasure_base
        {
            typedef utility::type_erasure_base base_type;
            typedef std::list<symbol const*> dependents_container_type;

            symbol_base(unsigned int type_id, std::string const& name, std::string const& file);

            virtual ~symbol_base() {}

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

            virtual bool is_equal(symbol_base const* other) const;

            virtual void fill_dependents() const {}

            virtual std::string get_type_name() const = 0;

        protected:
            std::string _name;
            std::string _file;
            mutable dependents_container_type _dependents;
        };

        template <typename Derived>
        using symbol = utility::type_erasure_impl<Derived, symbol_base>;
    }

    struct symbol
    {
        typedef impl::symbol_base::dependents_container_type dependents_container_type;

        symbol(std::string const& accessed_name = "")
            : impl()
            , _accessed_name(accessed_name)
        {}

        template <typename T>
        symbol(T const& entity)
            : impl(new T(entity))
        {}

        symbol(symbol const& entity)
            : impl(entity.impl)
            , _accessed_name(entity._accessed_name)
        {}

        symbol & operator = (symbol const& entity)
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

        impl::symbol_base * get_impl() const
        {
            return impl.get();
        }

        std::string get_type_name() const
        {
            return impl->get_type_name();
        }

        bool operator == (symbol const& other) const
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
            static_assert(!std::is_same<T, lazy_symbol>::value,
                "should not have to check for lazy_symbol, symbol_table should resolve these types after parsing");
        }

        std::shared_ptr<impl::symbol_base> impl;

        std::string _accessed_name;
    };
}

namespace std
{
    template <>
    struct hash<ffigen::symbol>
        : public hash<void const*>
    {
        typedef hash<void const*> base_type;

        size_t operator()(ffigen::symbol const& x) const;
    };
}

#endif // #if !defined(NODE_FFIGEN_PROCESS_SYMBOL_HPP)
