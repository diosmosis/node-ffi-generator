#if !defined(NODE_FFI_GENERATOR_PROCESS_PARSE_FILES_HPP)
#define NODE_FFI_GENERATOR_PROCESS_PARSE_FILES_HPP

#include <ffigen/process/symbol_table.hpp>
#include <ffigen/process/clang_facade.hpp>

namespace ffigen
{
    // TODO: should move to clang_facade object
    void parse_files(clang_facade const& clang, symbol_table & symbols);
}

#endif // #if !defined(NODE_FFI_GENERATOR_PROCESS_PARSE_FILES_HPP)