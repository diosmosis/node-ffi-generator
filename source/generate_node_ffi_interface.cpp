#include <ffigen/generate_node_ffi_interface.hpp>
#include <ffigen/process/symbol_table.hpp>
#include <ffigen/generate/interface_generator.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    void generate_node_ffi_interface(clang_facade const& clang, interface_generator const& generator)
    {
        debug() << "generate_node_ffi_interface()" << std::endl;

        symbol_table symbols;

        std::cout << "Parsing header files..." << std::endl;
        clang.parse_files(symbols);

        std::cout << "Generating JavaScript files..." << std::endl;
        generator(symbols);

        std::cout << "Done!" << std::endl;
    }
}