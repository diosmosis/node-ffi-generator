#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_FUNCTION_MAP_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_FUNCTION_MAP_GENERATOR_HPP

#include <ffigen/process/code_entity/function.hpp>
#include <ffigen/generate/generator.hpp>
#include <iosfwd>

namespace ffigen
{
    struct function_map_generator : impl::generator_base
    {
        function_map_generator(generator_factory const& factory, function const& entity, unsigned int indent = 0)
            : impl::generator_base(factory, indent)
            , entity(entity)
        {}

        void operator()(std::ostream & os) const;

        function const& entity;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_FUNCTION_MAP_GENERATOR_HPP)
