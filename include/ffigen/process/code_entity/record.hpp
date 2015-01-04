#if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_RECORD_HPP)
#define NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_RECORD_HPP

#include <ffigen/process/code_entity.hpp>
#include <ffigen/generate/generator.hpp>
#include <map>
#include <utility>
#include <iosfwd>

namespace ffigen
{
    struct record_entity : impl::code_entity<record_entity>
    {
        struct generator : impl::generator_base
        {
            generator(generator_factory const& factory, record_entity const& entity, unsigned int indent = 0)
                : impl::generator_base(factory, indent)
                , entity(entity)
            {}

            void operator()(std::ostream & os) const;
            void define_record_properties(record_entity const& entity, std::string const& js_access, std::ostream & os) const;

        protected:
            code_entity const* get_associated_type(code_entity const& entity) const;

            record_entity const& entity;
        };

        typedef impl::code_entity<record_entity> base_type;
        typedef std::list<std::pair<std::string, code_entity>> members_map_type; // TODO: rename members_container_type

        record_entity(std::string const& name, std::string const& file, members_map_type const& members,
                      bool is_anonymous, bool is_union)
            : base_type(name, file)
            , _members(members)
            , _is_anonymous(is_anonymous)
            , _is_union(is_union)
        {}

        members_map_type const& members() const
        {
            return _members;
        }

        bool is_anonymous() const
        {
            return _is_anonymous;
        }

        void fill_dependents() const;

        std::string get_type_name() const
        {
            return "record_entity";
        }

        bool is_union() const
        {
            return _is_union;
        }

        void deanonymize(std::string const& name)
        {
            _name = name;
            _is_anonymous = false;
        }

    private:
        members_map_type _members;
        bool _is_anonymous;
        bool _is_union;

        // store references to dependent records so we can reference typedef targets
        mutable std::list<code_entity> dependent_records;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_CODE_ENTITY_RECORD_HPP)