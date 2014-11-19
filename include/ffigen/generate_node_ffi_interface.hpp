#if !defined(NODE_FFI_GENERATOR_GENERATE_NODE_FFI_INTERFACE_HPP)
#define NODE_FFI_GENERATOR_GENERATE_NODE_FFI_INTERFACE_HPP

#include <string>
#include <list>

namespace ffigen
{
    void generate_node_ffi_interface(
        std::list<std::string> const& files,
        std::string const& src_root,
        std::string const& dest_root,
        std::list<std::string> const& include_directories
    );
}

#endif // #if !defined(NODE_FFI_GENERATOR_GENERATE_NODE_FFI_INTERFACE_HPP)
