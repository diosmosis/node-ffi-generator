#include <ffigen/process/symbol_table.hpp>
#include <ffigen/process/code_entity/lazy.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>
#include <stdexcept>

namespace ffigen
{
    using namespace utility::logs;

    code_entity & symbol_table::get(std::string fqn)
    {
        debug() << "symbol_table::get('" << fqn << "')" << std::endl;

        if (fqn.empty()) // sanity check
        {
            throw std::runtime_error("Empty fully qualified name sent to symbol_table::get()!");
        }

        fqn_map_type::iterator i = code_entities_by_fqn.find(fqn);

        if (i == code_entities_by_fqn.end())
        {
            all_entities.push_back(code_entity(lazy_code_entity(fqn, *this)));

            code_entity * new_entity = &all_entities.back();
            code_entities_by_fqn[fqn] = new_entity;

            debug() << "symbol_table::get(): adding new entity " << new_entity << std::endl;

            return *new_entity;
        }
        else
        {
            debug() << "symbol_table::get(): entity already exists " << &i->second << std::endl;

            return *i->second;
        }
    }

    symbol_table::types_by_file_container_type symbol_table::types_by_file(std::string const& src_root) const
    {
        symbol_table::types_by_file_container_type result;

        for (code_entity const& entity : all_entities)
        {
            if (!entity.get_impl())
            {
                warning() << "symbol_table::types_by_file(): "
                          << "null code_entity found in symbol table, something is missing from the "
                          << "source files or wasn't processed correctly"
                          << std::endl;

                continue;
            }

            if (entity.file().empty()
                || entity.is_anonymous()
            ) {
                continue;
            }

            // TODO: should be able to supply multiple source roots to program in case source is in multiple directories
            if (entity.file().compare(0, src_root.length(), src_root) != 0)
            {
                debug() << "skipping file '" << entity.file() << "', it does not belong to source root" << std::endl;

                continue;
            }

            types_by_file_container_type::iterator i = result.find(entity.file());

            if (i == result.end())
            {
                types_by_file_container_type::value_type value(entity.file(), code_entity_reference_list_type());
                i = result.insert(value).first;
            }

            i->second.push_back(entity);
        }

        return result;
    }

    void symbol_table::dfs(
        symbol_table::code_entity_reference_list_type const& types,
        std::string const& required_source_file,
        dfs_visitor_type const& visitor
    ) const
    {
        std::unordered_set<void *> visited;

        for (code_entity const& entity : types)
        {
            if (entity.is_anonymous())
            {
                continue;
            }

            dfs_visit_node(entity, required_source_file, visitor, visited);
        }
    }

    void symbol_table::dfs_visit_node(
        code_entity const& entity,
        std::string const& required_source_file,
        dfs_visitor_type const& visitor,
        std::unordered_set<void *> & visited
    ) const
    {
        if (!entity.get_impl())
        {
            warning() << "symbol_table::dfs_visit_node(): WARNING! found 'null' code_entity in symbol table." << std::endl;
            return;
        }

        if (visited.find(entity.get_impl()) != visited.end()
            || entity.file() != required_source_file
            || entity.name().empty()
        ) {
            return;
        }

        visited.insert(entity.get_impl());

        for (code_entity const* dependent_type : entity.dependents())
        {
            dfs_visit_node(*dependent_type, required_source_file, visitor, visited);
        }

        visitor(entity);
    }
}