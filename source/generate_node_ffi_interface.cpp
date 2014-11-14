#include <ffigen/generate_node_ffi_interface.hpp>
#include <ffigen/process/symbol_table.hpp>
#include <ffigen/process/parse_files.hpp>
#include <ffigen/generate/generate_js_files.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    void generate_node_ffi_interface(std::list<std::string> const& files, std::string const& src_root, std::string const& dest_root)
    {
        debug() << "generate_node_ffi_interface([size = " << files.size() << "], '" << src_root << "', '" << dest_root << "')" << std::endl;

        symbol_table symbols;

        parse_files(files, symbols);
        generate_js_files(symbols, src_root, dest_root);
    }
}
