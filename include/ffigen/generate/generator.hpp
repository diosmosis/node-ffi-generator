#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP

#include <functional>
#include <iosfwd>

namespace ffigen
{
    struct generator_factory;
    struct symbol;

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
            void output_preload_dependencies(std::ostream & os, ::ffigen::symbol const& entity, bool include_records = true) const;
            virtual ::ffigen::symbol const* get_associated_type(::ffigen::symbol const& entity) const;

            generator_factory const& factory;
            mutable unsigned int indent;
        };
    }

    typedef std::function<void (std::ostream&)> generator;
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP)
