#if !defined(NODE_FFI_GENERATOR_FFIGEN_GENERATE_INTERFACE_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_FFIGEN_GENERATE_INTERFACE_GENERATOR_HPP

#include <ffigen/process/symbol_table.hpp>
#include <boost/filesystem/path.hpp>
#include <string>

namespace ffigen
{
    struct interface_generator
    {
        void operator()(symbol_table & symbols) const;

        void validate() const;

        boost::filesystem::path src_root;
        boost::filesystem::path dest_root;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_FFIGEN_GENERATE_INTERFACE_GENERATOR_HPP)