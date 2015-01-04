#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_FACTORY_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_FACTORY_HPP

#include <ffigen/process/symbol.hpp>
#include <ffigen/generate/generator.hpp>

namespace ffigen
{
    struct generator_factory
    {
        generator make_for(symbol const& entity, unsigned int indent = 0) const;
        bool can_generate_for(symbol const& entity) const;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_FACTORY_HPP)
