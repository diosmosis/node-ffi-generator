#if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_TABLE_HPP)
#define NODE_FFI_GENERATOR_PROCESS_SYMBOL_TABLE_HPP

#include <ffigen/process/code_entity.hpp>
#include <ffigen/process/code_entity/function.hpp>
#include <clang/AST/Type.h>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>

namespace ffigen
{
    struct symbol_table
    {
        typedef std::unordered_map<std::string, code_entity *> fqn_map_type;
        typedef std::list<code_entity> code_entity_reference_list_type;
        typedef std::unordered_map<std::string, code_entity_reference_list_type> types_by_file_container_type;
        typedef std::function<void (code_entity const&)> dfs_visitor_type;

        symbol_table() {}

        code_entity & get(std::string fqn);

        types_by_file_container_type types_by_file(std::string const& src_root) const;

        void dfs(code_entity_reference_list_type const& types, std::string const& required_source_file,
            dfs_visitor_type const& visitor, dfs_visitor_type const& external_symbol_visitor = dfs_visitor_type()) const;

        std::list<code_entity> const& entities() const
        {
            return all_entities;
        }

    private:
        void dfs_visit_node(code_entity const& entity, std::string const& required_source_file,
            dfs_visitor_type const& visitor, dfs_visitor_type const& external_symbol_visitor,
            std::unordered_set<void *> & visited) const;

        std::list<code_entity> all_entities;
        fqn_map_type code_entities_by_fqn;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_TABLE_HPP)