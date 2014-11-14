#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_TYPEDEF_MAP_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_TYPEDEF_MAP_GENERATOR_HPP

#include <ffigen/process/code_entity/typedef.hpp>
#include <ffigen/generate/generator.hpp>
#include <iosfwd>

namespace ffigen
{
    struct typedef_map_generator : impl::generator_base
    {
        typedef_map_generator(generator_factory const& factory, typedef_entity const& entity, unsigned int indent = 0)
            : impl::generator_base(factory, indent)
            , entity(entity)
        {}

        void operator()(std::ostream & os) const;

        typedef_entity const& entity;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_TYPEDEF_MAP_GENERATOR_HPP)
