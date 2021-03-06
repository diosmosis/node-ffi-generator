#include <ffigen/process/symbol_table.hpp>
#include <ffigen/process/symbol/lazy_symbol.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>
#include <stdexcept>

namespace ffigen
{
    using namespace utility::logs;

    symbol * symbol_table::find(std::string const& fqn) const
    {
        fqn_map_type::const_iterator i = code_entities_by_fqn.find(fqn);

        if (i == code_entities_by_fqn.end()) {
            return nullptr;
        } else {
            return i->second;
        }
    }

    symbol & symbol_table::get(std::string fqn)
    {
        debug() << "symbol_table::get('" << fqn << "')" << std::endl;

        if (fqn.empty()) { // sanity check
            throw std::runtime_error("Empty fully qualified name sent to symbol_table::get()!");
        }

        fqn_map_type::iterator i = code_entities_by_fqn.find(fqn);

        if (i == code_entities_by_fqn.end()) {
            all_entities.push_back(symbol(lazy_symbol(fqn, *this)));

            symbol * new_entity = &all_entities.back();
            code_entities_by_fqn[fqn] = new_entity;

            debug() << "symbol_table::get(): adding new entity " << new_entity << std::endl;

            return *new_entity;
        } else {
            debug() << "symbol_table::get(): entity already exists " << &i->second << std::endl;

            return *i->second;
        }
    }

    symbol_table::types_by_file_container_type symbol_table::types_by_file(std::string const& src_root) const
    {
        symbol_table::types_by_file_container_type result;

        for (symbol const& entity : all_entities) {
            if (!entity) {
                warning() << "symbol_table::types_by_file(): "
                          << "null symbol '" << entity.accessed_name() << "' found in symbol table, something is missing from the "
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
            if (entity.file().compare(0, src_root.length(), src_root) != 0) {
                debug() << "skipping '" << entity.name() << "' in file '" << entity.file()
                        << "', it does not belong to source root" << std::endl;

                continue;
            }

            types_by_file_container_type::iterator i = result.find(entity.file());

            if (i == result.end()) {
                types_by_file_container_type::value_type value(entity.file(), symbol_reference_list_type());
                i = result.insert(value).first;
            }

            i->second.push_back(entity);
        }

        return result;
    }

    void symbol_table::dfs(
        symbol_table::symbol_reference_list_type const& types,
        std::string const& required_source_file,
        dfs_visitor_type const& visitor,
        dfs_visitor_type const& external_symbol_visitor
    ) const
    {
        std::unordered_set<symbol> visited;

        for (symbol const& entity : types) {
            if (entity.is_anonymous()) {
                continue;
            }

            dfs_visit_node(entity, required_source_file, visitor, external_symbol_visitor, visited);
        }
    }

    void symbol_table::dfs_visit_node(
        symbol const& entity,
        std::string const& required_source_file,
        dfs_visitor_type const& visitor,
        dfs_visitor_type const& external_symbol_visitor,
        std::unordered_set<symbol> & visited
    ) const
    {
        if (!entity) {
            debug() << "symbol_table::dfs_visit_node(): WARNING! found 'null' symbol '" << entity.accessed_name()
                    << "' in symbol table." << std::endl;
            return;
        }

        if (visited.find(entity) != visited.end()) {
            return;
        }

        visited.insert(entity);

        for (symbol const* dependent_type : entity.dependents()) {
            dfs_visit_node(*dependent_type, required_source_file, visitor, external_symbol_visitor, visited);
        }

        if (entity.name().empty()
            || entity.file().empty()
            || entity.is_anonymous()
        ) {
            return;
        }

        if (!required_source_file.empty()
            && !entity.file().empty()
            && entity.file() != required_source_file
        ) {
            if (external_symbol_visitor) {
                external_symbol_visitor(entity);
            }

            return;
        }

        visitor(entity);
    }

    static symbol & get_real_entity(symbol const& lazy)
    {
        // TODO: const_cast bad, no do const_cast (same as below)
        symbol & impl = const_cast<symbol &>(static_cast<lazy_symbol *>(lazy.get_impl())->get_impl());

        if (impl.is_a<lazy_symbol>()) {
            return get_real_entity(impl);
        } else {
            return impl;
        }
    }

    void symbol_table::resolve_lazy_symbols()
    {
        // replace all lazy symbols in the symbol map
        for (auto & pair : code_entities_by_fqn) {
            if (!*pair.second) {
                continue;
            }

            if (pair.second->is_a<lazy_symbol>()) {
                pair.second = &get_real_entity(*pair.second);
            }
        }

        // replace dependents
        for (symbol & entity : all_entities) {
            if (!entity
                || entity.is_a<lazy_symbol>()
            ) {
                continue;
            }

            for (symbol const* dependent : entity.dependents()) {
                symbol * mutable_dependent = const_cast<symbol *>(dependent);

                if (dependent->is_a<lazy_symbol>()) {
                    *mutable_dependent = get_real_entity(*dependent);
                }
            }
        }
    }
}