#if !defined(NODE_FFIGEN_UTILITY_TYPE_ERASURE_BASE_HPP)
#define NODE_FFIGEN_UTILITY_TYPE_ERASURE_BASE_HPP

#include <stdint.h>

namespace ffigen { namespace utility
{
    namespace impl
    {
        template <typename T>
        inline void id_function()
        {
            // empty
        }

        template <unsigned int FunctionPointerSize> struct int_type_of;

        template <>
        struct int_type_of<4>
        {
            typedef uint32_t type;
        };

        template <>
        struct int_type_of<8>
        {
            typedef uint64_t type;
        };

        typedef void (*type_id_value_type)();
        typedef int_type_of<sizeof(type_id_value_type)>::type type_id_int_type;

        template <typename T>
        struct type_id_generator
        {
            typedef type_id_generator<T> self_type;

            static type_id_int_type get_id()
            {
                type_id_value_type result = &id_function<T>;
                return *reinterpret_cast<type_id_int_type *>(&result);
            }
        };
    }

    template <typename T>
    inline impl::type_id_int_type get_type_id()
    {
        return impl::type_id_generator<T>::get_id();
    }

    struct type_erasure_base
    {
        type_erasure_base()
            : _type_id(0)
        {}

        type_erasure_base(impl::type_id_int_type _type_id)
            : _type_id(_type_id)
        {}

        template <typename T>
        bool is_a() const {
            return get_type_id<T>() == _type_id;
        }

        bool is_same_class_as(type_erasure_base const* other) const {
            return _type_id == other->_type_id;
        }

    protected:
        impl::type_id_int_type _type_id;
    };

    template <typename Derived, typename Base>
    struct type_erasure_impl : Base
    {
        template <typename... ArgumentTypes>
        type_erasure_impl(ArgumentTypes... args)
            : Base(get_type_id<Derived>(), args...)
        {}
    };
}}

#endif // #if !defined(NODE_FFIGEN_UTILITY_TYPE_ERASURE_BASE_HPP)