#if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_RECORD_HPP)
#define NODE_FFI_GENERATOR_PROCESS_SYMBOL_RECORD_HPP

#include <ffigen/process/symbol.hpp>
#include <ffigen/generate/generator.hpp>
#include <map>
#include <utility>
#include <iosfwd>

namespace ffigen
{
    struct record_symbol : impl::symbol<record_symbol>
    {
        struct generator : impl::generator_base
        {
            generator(generator_factory const& factory, record_symbol const& entity, unsigned int indent = 0)
                : impl::generator_base(factory, indent)
                , entity(entity)
            {}

            void operator()(std::ostream & os) const;
            void define_record_properties(record_symbol const& entity, std::string const& js_access, std::ostream & os) const;

        protected:
            symbol const* get_associated_type(symbol const& entity) const;

            record_symbol const& entity;
        };

        typedef impl::symbol<record_symbol> base_type;
        typedef std::list<std::pair<std::string, symbol>> members_map_type; // TODO: rename members_container_type

        record_symbol(std::string const& name, std::string const& file, members_map_type const& members,
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
            return "record_symbol";
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
        mutable std::list<symbol> dependent_records;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_RECORD_HPP)