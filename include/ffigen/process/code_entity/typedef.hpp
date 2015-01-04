#if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_TYPEDEF_HPP)
#define NODE_FFI_GENERATOR_CODE_ENTITY_TYPEDEF_HPP

#include <ffigen/process/code_entity.hpp>
#include <ffigen/generate/generator.hpp>
#include <iosfwd>

namespace ffigen
{
    struct typedef_entity : impl::code_entity<typedef_entity>
    {
        struct generator : impl::generator_base
        {
            generator(generator_factory const& factory, typedef_entity const& entity, unsigned int indent = 0)
                : impl::generator_base(factory, indent)
                , entity(entity)
            {}

            void operator()(std::ostream & os) const;

            typedef_entity const& entity;
        };

        typedef impl::code_entity<typedef_entity> base_type;

        typedef_entity(std::string const& name, std::string const& file, code_entity const& type)
            : base_type(name, file)
            , _type(type)
        {}

        code_entity const& alias_type() const
        {
            return _type;
        }

        void fill_dependents() const
        {
            _dependents.push_back(&_type);
        }

        std::string get_type_name() const
        {
            return "typedef_entity";
        }
    private:
        code_entity _type;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_TYPEDEF_HPP)