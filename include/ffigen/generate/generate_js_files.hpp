#if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATE_JS_FILES_HPP)
#define NODE_FFI_GENERATOR_GENERATE_GENERATE_JS_FILES_HPP

#include <ffigen/process/symbol_table.hpp>
#include <string>

namespace ffigen
{
    void generate_js_files(symbol_table & symbols, std::string const& src_root, std::string const& dest_root);
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_GENERATE_JS_FILES_HPP)