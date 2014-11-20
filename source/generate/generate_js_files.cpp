#include <ffigen/generate/generate_js_files.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/generate/generator/library_load_generator.hpp>
#include <ffigen/utility/logger.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;
    namespace fs = boost::filesystem;

    fs::path relpath(fs::path p, fs::path root)
    {
        p = fs::canonical(p);
        root = fs::canonical(root);

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
        os << "var ref = require('ref'),\n"
           << "    RefArray = require('ref-array'),\n"
           << "    Struct = require('ref-struct'),\n"
           << "    Union = require('ref-union')";

        if (!is_root_file)
        {
            os << ",\n    _library = require('./" << interface_root.string() << "');\n\n";
        }
        else
        {
            os << ";\n\n";
        }
    }

    void generate_js_files(symbol_table & symbols, std::string const& src_root_str, std::string const& dest_root_str)
    {
        debug() << "generate_js_files(" << &symbols << ", '" << src_root_str << "', '" << dest_root_str << "')" << std::endl;

        std::list<std::string> modules;

        fs::path src_root(src_root_str),
                 dest_root(dest_root_str);

        generator_factory factory;

        // generate individual JS files
        for (auto const& pair : symbols.types_by_file(src_root_str))
        {
            debug() << "generate_js_files(): processing symbols in '" << pair.first << "'" << std::endl;

            fs::path src_file = fs::path(pair.first);
            fs::path dest_file = get_js_dest_file_for(src_file, src_root, dest_root);

            debug() << "generate_js_files(): [ src_file = '" << src_file.string() << "', dest_file = '" << dest_file.string()
                    << "']" << std::endl;

            fs::create_directories(dest_file.parent_path());

            // TODO: check if already exists (should do once before any generation)
            // must create the file before using it in relpath, otherwise boost::canonical will throw
            std::ofstream out(dest_file.string(), std::ios_base::out);

            fs::path rel_module_path = relpath(dest_file, dest_root);
            rel_module_path.replace_extension("");
            modules.push_back(rel_module_path.string());

            debug() << "generate_js_files(): [ rel_module_path = '" << rel_module_path.string() << "' ]" << std::endl;

            start_new_file(out, dest_file, dest_root);

            debug() << "generate_js_files(): file started" << std::endl;

            symbols.dfs(pair.second, src_file.string(), [&out, &factory] (code_entity const& entity) {
                factory.make_for(entity)(out);
            });

            debug() << "generate_js_files(): dfs finished" << std::endl;
        }

        // generate main JS file with ffi.Library factory function
        fs::path dest_index_js = dest_root / "index.js";

        debug() << "generate_js_files(): [ dest_index_js = '" << dest_index_js << "' ]" << std::endl;

        std::ofstream out(dest_index_js.string(), std::ios_base::out); // TODO: check if already exists

        start_new_file(out, dest_index_js, dest_root, true);

        library_load_generator library_gen(factory, symbols, modules);
        library_gen(out);

        debug() << "generate_js_files(): done" << std::endl;
    }
}
