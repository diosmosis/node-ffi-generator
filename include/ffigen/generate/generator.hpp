#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP

#include <functional>
#include <iosfwd>

namespace ffigen
{
    struct generator_factory;

    namespace impl
    {
        struct generator_base
        {
            generator_base(generator_factory const& factory, unsigned int indent = 0)
                : factory(factory)
                , indent(indent)
            {}

            void newline(std::ostream & os) const;

            generator_factory const& factory;
            unsigned int indent;
        };
    }

    typedef std::function<void (std::ostream&)> generator;
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_HPP)
