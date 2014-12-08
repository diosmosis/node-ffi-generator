#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP

#include <functional>
#include <iosfwd>

namespace ffigen
{
    struct generator_factory;
    struct code_entity;

    namespace impl
    {
        struct generator_base
        {
            generator_base(generator_factory const& factory, unsigned int indent = 0)
                : factory(factory)
                , indent(indent)
            {}
            virtual ~generator_base() {}

            void newline(std::ostream & os) const;
            void newline(std::ostream & os, unsigned int indent) const;

        protected:
            void output_preload_dependencies(std::ostream & os, ::ffigen::code_entity const& entity, bool include_records = true) const;
            virtual ::ffigen::code_entity const* get_associated_type(::ffigen::code_entity const& entity) const;

            generator_factory const& factory;
            mutable unsigned int indent;
        };
    }

    typedef std::function<void (std::ostream&)> generator;
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP)
