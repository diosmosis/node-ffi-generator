#if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_UNION_HPP)
#define NODE_FFI_GENERATOR_CODE_ENTITY_UNION_HPP

#include <ffigen/process/code_entity.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <map>

namespace ffigen
{
    struct union_entity : impl::code_entity<union_entity>
    {
        typedef impl::code_entity<union_entity> base_type;
        typedef std::map<std::string, code_entity const*> variants_map_type;

        union_entity(std::string const& name, std::string const& file, variants_map_type const& variants,
                     bool is_anonymous)
            : base_type(name, file)
            , _variants(variants)
            , _is_anonymous(is_anonymous)
        {
            for (auto const& pair : variants)
            {
                _dependents.push_back(pair.second);
            }
        }

        variants_map_type const& variants() const
        {
            return _variants;
        }

        bool is_anonymous() const
        {
            return _is_anonymous;
        }

    private:
        variants_map_type _variants;
        bool _is_anonymous;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_UNION_HPP)
