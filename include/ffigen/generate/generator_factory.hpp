#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_FACTORY_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_FACTORY_HPP

#include <ffigen/process/code_entity.hpp>
#include <ffigen/generate/generator.hpp>

namespace ffigen
{
    struct generator_factory
    {
        generator make_for(code_entity const& entity, unsigned int indent = 0) const;
        bool can_generate_for(code_entity const& entity) const;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_FACTORY_HPP)
