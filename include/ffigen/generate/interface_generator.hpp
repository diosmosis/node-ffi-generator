#if !defined(NODE_FFI_GENERATOR_FFIGEN_GENERATE_INTERFACE_GENERATOR_HPP)
#define NODE_FFI_GENERATOR_FFIGEN_GENERATE_INTERFACE_GENERATOR_HPP

#include <ffigen/process/symbol_table.hpp>
#include <boost/filesystem/path.hpp>
#include <string>
#include <set>
#include <iosfwd>

namespace ffigen
{
    struct interface_generator
    {
        typedef boost::filesystem::path path_type;

        void operator()(symbol_table & symbols) const;

        void validate() const;

        path_type src_root;
        path_type dest_root;

    private:
        path_type get_js_dest_file_for(path_type const& src_file) const;
        void start_new_file(std::ostream & os, path_type const& path, bool is_root_file = false) const;
        void end_file(std::ostream & os, std::set<path_type> const& external_symbol_paths,
            path_type const& dest_file) const;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_FFIGEN_GENERATE_INTERFACE_GENERATOR_HPP)