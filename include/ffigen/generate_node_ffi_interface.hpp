#if !defined(NODE_FFI_GENERATOR_GENERATE_NODE_FFI_INTERFACE_HPP)
#define NODE_FFI_GENERATOR_GENERATE_NODE_FFI_INTERFACE_HPP

#include <ffigen/process/clang_facade.hpp>
#include <string>
#include <list>

namespace ffigen
{
    void generate_node_ffi_interface(
        clang_facade const& clang,
        std::string const& src_root,
        std::string const& dest_root
    );
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_NODE_FFI_INTERFACE_HPP)
