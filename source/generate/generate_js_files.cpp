#include <ffigen/generate/generate_js_files.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/generate/generator/library_load_generator.hpp>
#include <ffigen/process/code_entity/reference.hpp>
#include <ffigen/process/code_entity/array_entity.hpp>
#include <ffigen/process/code_entity/record.hpp>
#include <ffigen/process/code_entity/typedef.hpp>
#include <ffigen/process/code_entity/lazy.hpp>
#include <ffigen/utility/logger.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <deque>

namespace ffigen
{
    using namespace utility::logs;
    namespace fs = boost::filesystem;

    struct code_entity_compare
    {
        bool operator() (code_entity const& x, code_entity const& y) const
        {
            return x.name() < y.name();
        }
    };

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

    fs::path get_js_dest_file_for(fs::path const& src_file, fs::path const& src_root, fs::path const& dest_root)
    {
        fs::path src_relative = relpath(src_file, src_root);
        return dest_root / src_relative.parent_path() / (src_relative.stem().string() + ".js");
    }

    void start_new_file(std::ostream & os, fs::path const& path, fs::path const& dest_root, bool is_root_file = false)
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

    bool directory_contains(fs::path const& dir, fs::path const& p)
    {
        return std::equal(dir.begin(), dir.end(), p.begin());
    }

    void end_file(std::ostream & os, std::set<fs::path> const& external_symbol_paths, fs::path const& dest_file,
                  fs::path const& src_root, fs::path const& dest_root)
    {
        os << "function loadDependentSymbols() {\n";
        for (auto const& external_path : external_symbol_paths) {
            if (directory_contains(src_root, external_path)) {
                fs::path external_dest_file = dest_root / relpath(external_path, src_root).replace_extension(".js");
                fs::path to_file = relpath(external_dest_file, dest_file.parent_path());

                os << "    require('./" << to_file.string() << "');\n";
            }
        }
        os << "}\n";
        os << std::flush;
    }

    void generate_js_files(symbol_table & symbols, std::string const& src_root_str, std::string const& dest_root_str)
    {
        debug() << "generate_js_files(" << &symbols << ", '" << src_root_str << "', '" << dest_root_str << "')" << std::endl;

        std::list<std::string> modules;
        // TODO: better to use unordered_set, but hash-ing code_entities doesn't seem to work. some symbols cannot
        //       be found.
        std::set<code_entity, code_entity_compare> external_dependent_symbols,
                                                   visited_symbols;

        fs::path src_root(src_root_str),
                 dest_root(dest_root_str);

        generator_factory factory;

        // generate individual JS files
        for (auto const& pair : symbols.types_by_file(src_root_str)) {
            debug() << "generate_js_files(): processing symbols in '" << pair.first << "'" << std::endl;

            std::set<fs::path> this_modules_external_symbols;

            std::cout << "Generating binding for '" << pair.first << "'..." << std::endl;

            fs::path src_file = fs::path(pair.first);
            fs::path dest_file = get_js_dest_file_for(src_file, src_root, dest_root);

            debug() << "generate_js_files(): [ src_file = '" << src_file.string() << "', dest_file = '" << dest_file.string()
                    << "']" << std::endl;

            fs::create_directories(dest_file.parent_path());

            // TODO: check if already exists (should do once before any generation)
            std::ofstream out(dest_file.string(), std::ios_base::out);

            fs::path rel_module_path = relpath(dest_file, dest_root);
            rel_module_path.replace_extension("");
            modules.push_back(rel_module_path.string());

            debug() << "generate_js_files(): [ rel_module_path = '" << rel_module_path.string() << "' ]" << std::endl;

            start_new_file(out, dest_file, dest_root);

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

            end_file(out, this_modules_external_symbols, dest_file, src_root, dest_root);

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

        start_new_file(out, dest_index_js, dest_root, true);

        library_load_generator library_gen(factory, symbols, modules, external_dependent_symbols_list);
        library_gen(out);

        debug() << "generate_js_files(): done" << std::endl;
    }
}
