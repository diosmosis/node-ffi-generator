#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_RECORD_MAP_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATOR_RECORD_MAP_GENERATOR_HPP

#include <ffigen/process/code_entity/record.hpp>
#include <ffigen/generate/generator.hpp>
#include <iosfwd>

namespace ffigen
{
    struct record_map_generator : impl::generator_base
    {
        record_map_generator(generator_factory const& factory, record_entity const& entity, unsigned int indent = 0)
            : impl::generator_base(factory, indent)
            , entity(entity)
        {}

        void operator()(std::ostream & os) const;
        void define_record_properties(record_entity const& entity, std::string const& js_access, std::ostream & os) const;

    protected:
        code_entity const* get_associated_type(code_entity const& entity) const;

        record_entity const& entity;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATOR_RECORD_MAP_GENERATOR_HPP)
