#if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_STRUCT_HPP)
#define NODE_FFI_GENERATOR_CODE_ENTITY_STRUCT_HPP

#include <ffigen/process/code_entity.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <map>

namespace ffigen
{
    struct struct_entity : impl::code_entity<struct_entity>
    {
        typedef impl::code_entity<struct_entity> base_type;
        typedef std::map<std::string, code_entity> members_map_type;

        struct_entity(std::string const& name, std::string const& file, members_map_type const& members,
                      bool is_anonymous)
            : base_type(name, file)
            , _members(members)
            , _is_anonymous(is_anonymous)
        {}

        members_map_type const& members() const
        {
            return _members;
        }

        bool is_anonymous() const
        {
            return _is_anonymous;
        }

        void fill_dependents() const
        {
            for (auto const& pair : _members)
            {
                _dependents.push_back(&pair.second);
            }
        }

        std::string get_type_name() const
        {
            return "struct_entity";
        }
    private:
        members_map_type _members;
        bool _is_anonymous;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_CODE_ENTITY_STRUCT_HPP)
