#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_LIBRARY_LOAD_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_LIBRARY_LOAD_GENERATOR_HPP

#include <ffigen/process/symbol_table.hpp>
#include <ffigen/generate/generator.hpp>
#include <iosfwd>
#include <list>
#include <string>

namespace ffigen
{
    struct library_load_generator : impl::generator_base
    {
        library_load_generator(generator_factory const& factory, symbol_table const& symbols,
                               std::list<std::string> const& modules, unsigned int indent = 0)
            : impl::generator_base(factory, indent)
            , symbols(symbols)
            , modules(modules)
        {}

        void operator()(std::ostream & os) const;

        symbol_table const& symbols;
        std::list<std::string> const& modules;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_LIBRARY_LOAD_GENERATOR_HPP)
