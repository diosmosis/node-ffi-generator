#include <ffigen/generate/interface_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/generate/generator/library_load_generator.hpp>
#include <ffigen/process/code_entity/reference.hpp>
#include <ffigen/process/code_entity/array_entity.hpp>
#include <ffigen/process/code_entity/record.hpp>
#include <ffigen/process/code_entity/typedef.hpp>
#include <ffigen/process/code_entity/lazy.hpp>
#include <ffigen/utility/error_codes.hpp>
#include <ffigen/utility/exceptions.hpp>
#include <ffigen/utility/logger.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <unordered_set>

namespace ffigen
{
    using namespace utility::logs;
    namespace fs = boost::filesystem;

    namespace detail
    {
        bool directory_contains(fs::path const& dir, fs::path const& p)
        {
            return std::equal(dir.begin(), dir.end(), p.begin());
        }
    }

    fs::path relpath(fs::path p, fs::path root)
    {
        p = fs::absolute(p);
        root = fs::absolute(root);

        fs::path::iterator i = p.begin(), j = root.begin();
        for (; i != p.end() && j != root.end() && *i == *j;
             ++i, ++j)
        {
            // empty
        }

        std::stringstream ss;

        for (; j != root.end(); ++j) {
            ss << "../";
        }

        fs::path::iterator last = p.end(); --last;
        for (; i != p.end(); ++i) {
            ss << i->string();

            if (i != last) {
                ss << "/";
            }
        }

        return fs::path(ss.str());
    }

    fs::path interface_generator::get_js_dest_file_for(fs::path const& src_file) const
    {
        fs::path src_relative = relpath(src_file, src_root);
        return dest_root / src_relative.parent_path() / (src_relative.stem().string() + ".js");
    }

    void interface_generator::start_new_file(std::ostream & os, fs::path const& path, bool is_root_file) const
    {
        fs::path interface_root = relpath(dest_root, path.parent_path());
        os << "var ffi = require('ffi'),\n"
           << "    ref = require('ref'),\n"
           << "    RefArray = require('ref-array'),\n"
           << "    Struct = require('ref-struct'),\n"
           << "    Union = require('ref-union')";

        if (!is_root_file) {
            os << ",\n    _library = require('./" << interface_root.string() << "');\n\n";
            os << "loadDependentSymbols();\n\n";
        } else {
            os << ";\n\n";
        }

        os << std::flush;
    }

    void interface_generator::end_file(std::ostream & os, std::set<fs::path> const& external_symbol_paths,
                                       fs::path const& dest_file) const
    {
        os << "function loadDependentSymbols() {\n";
        for (auto const& external_path : external_symbol_paths) {
            if (detail::directory_contains(src_root, external_path)) {
                fs::path external_dest_file = dest_root / relpath(external_path, src_root).replace_extension(".js");
                fs::path to_file = relpath(external_dest_file, dest_file.parent_path());

                os << "    require('./" << to_file.string() << "');\n";
            }
        }
        os << "}\n";
        os << std::flush;
    }

    void interface_generator::operator()(symbol_table & symbols) const
    {
        debug() << "generate_js_files(" << &symbols << ", " << src_root << ", " << dest_root << ")" << std::endl;

        std::list<std::string> modules;

        std::unordered_set<code_entity> external_dependent_symbols,
                                        visited_symbols;

        generator_factory factory;

        // generate individual JS files
        for (auto const& pair : symbols.types_by_file(src_root.string())) {
            debug() << "generate_js_files(): processing symbols in '" << pair.first << "'" << std::endl;

            std::set<fs::path> this_modules_external_symbols;

            std::cout << "Generating binding for '" << pair.first << "'..." << std::endl;

            fs::path src_file = fs::path(pair.first);
            fs::path dest_file = get_js_dest_file_for(src_file);

            debug() << "generate_js_files(): [ src_file = '" << src_file.string() << "', dest_file = '" << dest_file.string()
                    << "']" << std::endl;

            fs::create_directories(dest_file.parent_path());

            // TODO: check if already exists (should do once before any generation)
            std::ofstream out(dest_file.string(), std::ios_base::out);

            fs::path rel_module_path = relpath(dest_file, dest_root);
            rel_module_path.replace_extension("");
            modules.push_back(rel_module_path.string());

            debug() << "generate_js_files(): [ rel_module_path = '" << rel_module_path.string() << "' ]" << std::endl;

            start_new_file(out, dest_file);

            debug() << "generate_js_files(): file started" << std::endl;

            symbols.dfs(pair.second, src_file.string(), [&out, &factory, &visited_symbols] (code_entity const& entity) {
                    visited_symbols.insert(entity);

                    factory.make_for(entity)(out);
                },
                [&external_dependent_symbols, &this_modules_external_symbols] (code_entity const& entity) {
                    external_dependent_symbols.insert(entity);
                    this_modules_external_symbols.emplace(entity.file());
                }
            );

            end_file(out, this_modules_external_symbols, dest_file);

            debug() << "generate_js_files(): dfs finished" << std::endl;
        }

        for (auto const& symbol : visited_symbols) {
            auto i = external_dependent_symbols.find(symbol);
            if (i != external_dependent_symbols.end()) {
                external_dependent_symbols.erase(i);
            }
        }

        visited_symbols.clear();

        // TODO: annoying that I have to create a list... should use any_iterator. w/o RTTI. possible?
        std::list<code_entity> external_dependent_symbols_list(
            external_dependent_symbols.begin(),
            external_dependent_symbols.end()
        );

        external_dependent_symbols.clear();

        // generate main JS file with ffi.Library factory function
        std::cout << "Generating binding entry point..." << std::endl;

        fs::path dest_index_js = dest_root / "index.js";

        debug() << "generate_js_files(): [ dest_index_js = '" << dest_index_js << "' ]" << std::endl;

        std::ofstream out(dest_index_js.string(), std::ios_base::out); // TODO: check if already exists

        start_new_file(out, dest_index_js, true);

        library_load_generator library_gen(factory, symbols, modules, external_dependent_symbols_list);
        library_gen(out);

        debug() << "generate_js_files(): done" << std::endl;
    }

    void interface_generator::validate() const
    {
        if (dest_root.empty()) {
            throw fatal_error("Required option 'dest' not supplied.", error_codes::MISSING_ARGUMENT);
        }

        if (src_root.empty()) {
            throw fatal_error("Required option 'src-root' not supplied.", error_codes::MISSING_ARGUMENT);
        }
    }
}