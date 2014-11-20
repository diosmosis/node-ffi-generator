#include <ffigen/generate_node_ffi_interface.hpp>
#include <ffigen/process/symbol_table.hpp>
#include <ffigen/process/parse_files.hpp>
#include <ffigen/generate/generate_js_files.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    void generate_node_ffi_interface(std::list<std::string> const& files, std::string const& src_root,
        std::string const& dest_root, std::list<std::string> const& include_directories)
    {
        debug() << "generate_node_ffi_interface([size = " << files.size() << "], '" << src_root << "', '"
                << dest_root << "', [size = " << include_directories.size() << "])" << std::endl;

        symbol_table symbols;

        std::cout << "Parsing header files..." << std::endl;
        parse_files(files, symbols, include_directories);

        std::cout << "Generating JavaScript files..." << std::endl;
        generate_js_files(symbols, src_root, dest_root);

        std::cout << "Done!" << std::endl;
    }
}