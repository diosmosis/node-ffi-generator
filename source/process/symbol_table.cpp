#include <ffigen/process/symbol_table.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    code_entity & symbol_table::get(std::string fqn)
    {
        debug() << "symbol_table::get('" << fqn << "')" << std::endl;

        if (fqn.empty())
        {
            fqn = make_anonymous_entity_id();

            debug() << "symbol_table::get(): making anonyous entity id '" << fqn << "'" << std::endl;
        }

        fqn_map_type::iterator i = code_entities_by_fqn.find(fqn);

        if (i == code_entities_by_fqn.end())
        {
            all_entities.push_back(code_entity());

            code_entity * new_entity = &all_entities.back();
            code_entities_by_fqn[fqn] = new_entity;

            debug() << "symbol_table::get(): adding new entity " << new_entity << std::endl;

            return *new_entity;
        }
        else
        {
            debug() << "symbol_table::get(): adding new entity " << &i->second << std::endl;

            return *i->second;
        }
    }

    std::string symbol_table::make_anonymous_entity_id()
    {
        ++anonymous_entity_count;

        return std::string("_anonymous_") + std::to_string(anonymous_entity_count);
    }

    symbol_table::types_by_file_container_type symbol_table::types_by_file(std::string const& src_root) const
    {
        symbol_table::types_by_file_container_type result;

        for (code_entity const& entity : all_entities)
        {
            if (!entity)
            {
                warning() << "symbol_table::types_by_file(): "
                          << "null code_entity found in symbol table, something is missing from the "
                          << "source files or wasn't processed correctly"
                          << std::endl;

                continue;
            }

            if (entity.file().empty())
            {
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
        for (code_entity const& entity : types)
        {
            dfs_visit_node(entity, required_source_file, visitor);
        }
    }

    void symbol_table::dfs_visit_node(
        code_entity const& entity,
        std::string const& required_source_file,
        dfs_visitor_type const& visitor
    ) const
    {
        if (entity.file() != required_source_file || entity.name().empty())
        {
            return;
        }

        for (code_entity const* dependent_type : entity.dependents())
        {
            dfs_visit_node(*dependent_type, required_source_file, visitor);
        }

        visitor(entity);
    }
}
