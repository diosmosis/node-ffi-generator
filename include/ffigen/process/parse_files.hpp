#if !defined(NODE_FFI_GENERATOR_PROCESS_PARSE_FILES_HPP)
#define NODE_FFI_GENERATOR_PROCESS_PARSE_FILES_HPP

#include <ffigen/process/symbol_table.hpp>

namespace ffigen
{
    void parse_files(std::list<std::string> const& files, symbol_table & symbols);
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_PARSE_FILES_HPP)