#if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_TABLE_HPP)
#define NODE_FFI_GENERATOR_PROCESS_SYMBOL_TABLE_HPP

#include <ffigen/process/symbol.hpp>
#include <ffigen/process/symbol/function_symbol.hpp>
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
        typedef std::unordered_map<std::string, symbol *> fqn_map_type;
        typedef std::list<symbol> symbol_reference_list_type;
        typedef std::unordered_map<std::string, symbol_reference_list_type> types_by_file_container_type;
        typedef std::function<void (symbol const&)> dfs_visitor_type;

        symbol_table() {}

        symbol & get(std::string fqn);
        symbol * find(std::string const& fqn) const;

        types_by_file_container_type types_by_file(std::string const& src_root) const;

        void dfs(symbol_reference_list_type const& types, std::string const& required_source_file,
            dfs_visitor_type const& visitor, dfs_visitor_type const& external_symbol_visitor = dfs_visitor_type()) const;

        void resolve_lazy_symbols();

        std::list<symbol> const& entities() const
        {
            return all_entities;
        }

    private:
        void dfs_visit_node(symbol const& entity, std::string const& required_source_file,
            dfs_visitor_type const& visitor, dfs_visitor_type const& external_symbol_visitor,
            std::unordered_set<symbol> & visited) const;

        std::list<symbol> all_entities;
        fqn_map_type code_entities_by_fqn;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_SYMBOL_TABLE_HPP)