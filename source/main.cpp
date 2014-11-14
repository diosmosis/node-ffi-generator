#include <ffigen/generate_node_ffi_interface.hpp>
#include <ffigen/utility/logger.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <stdexcept>

namespace fs = boost::filesystem;

enum error_codes
{
    UNKNOWN_ERROR = -1,
    SUCCESS = 0,
    MISSING_ARGUMENT = 1,
    INVALID_ARGUMENT = 2
};

void recursive_glob_for_headers(std::list<std::string> & files, fs::path const& path)
{
    fs::recursive_directory_iterator i(path), end;
    for (; i != end; ++i)
    {
        fs::path const& path(i->path());

        std::string extension = path.extension().string();
        if (extension == ".h" || extension == ".hpp" || extension == ".hxx")
        {
            files.push_back(path.string());
        }
    }
}

//! required options:
//!   --file=path/to/file
//!     if directory, globs directory for .h/.hpp/.hxx files
//!     can be specified multiple times
//!   --dest=path/to/destination
//!     must be directory
//!   --src-root=path/to/root
//!     must be directory
//! TODO: add log level argument
int main(int argc, char ** argv)
{
    using namespace ffigen::utility::logs;

    set_level(levels::DEBUG);

    std::string FILE_OPTION_PREFIX("--file="),
                DEST_OPTION_PREFIX("--dest="),
                SRC_OPTION_PREFIX("--src-root=");

    std::list<std::string> files_to_process;
    std::string dest, src_root;

    for (int i = 0; i < argc; ++i)
    {
        std::string arg(argv[i]);

        if (arg == "--help")
        {
            std::cout << "Generates node-ffi JS bindings using one or more C header files. Embeds clang's parser to do so.\n\n";
            std::cout << "Usage: node-ffi-generator --file=path/to/headers --dest=path/to/jsfiles/destination --src-root=path/headers/root/dir\n\n";
            std::cout << "Options:\n";
            std::cout << "  --file=path/to/file      C header file or directory with header files to process. If a directory,\n";
            std::cout << "                           it is searched recursively for .h, .hpp and .hxx files.\n";
            std::cout << "  --dest=path/to/dir       The destination directory for the mapping files.\n";
            std::cout << "  --src-root=path/to/dir   The root directory of the C library that's being mapped. Helps to determine\n";
            std::cout << "                           the destination's directory structure.\n";

            return SUCCESS;
        }
        else if (boost::starts_with(arg, FILE_OPTION_PREFIX)) // TODO should check if --file=...
        {
            std::string value = arg.substr(FILE_OPTION_PREFIX.length());

            // TODO: should support glob expressions
            fs::path path(value);
            if (fs::is_directory(path))
            {
                recursive_glob_for_headers(files_to_process, path);
            }
            else if (fs::is_regular_file(path))
            {
                files_to_process.push_back(value);
            }
            else
            {
                std::cout << "ERROR: '" << value << "' supplied in --file option does not exist.\n";
                return INVALID_ARGUMENT;
            }

            debug() << "Processed --file argument '" << value << "'" << std::endl;
        }
        else if (boost::starts_with(arg, DEST_OPTION_PREFIX))
        {
            std::string value = arg.substr(DEST_OPTION_PREFIX.length());

            if (!fs::is_directory(fs::path(value)))
            {
                std::cout << "ERROR: Destination '" << value << "' is not a valid directory.\n";
                return INVALID_ARGUMENT;
            }

            dest = value;

            debug() << "Processed --dest argument '" << dest << "'" << std::endl;
        }
        else if (boost::starts_with(arg, SRC_OPTION_PREFIX))
        {
            std::string value = arg.substr(SRC_OPTION_PREFIX.length());

            if (!fs::is_directory(fs::path(value)))
            {
                std::cout << "ERROR: Source root directory '" << value << "' is not a valid directory.\n";
                return INVALID_ARGUMENT;
            }

            src_root = value;

            debug() << "Processed --src-root argument '" << src_root << "'" << std::endl;
        }
    }

    if (files_to_process.empty())
    {
        std::cout << "ERROR: Required option 'file' not supplied.\n";
        return MISSING_ARGUMENT;
    }

    if (dest.empty())
    {
        std::cout << "ERROR: Required option 'dest' not supplied.\n";
        return MISSING_ARGUMENT;
    }

    if (src_root.empty())
    {
        std::cout << "ERROR: Required option 'src-root' not supplied.\n";
        return MISSING_ARGUMENT;
    }

    // generate the interface
    try
    {
        ffigen::generate_node_ffi_interface(files_to_process, src_root, dest);
    }
    catch (std::exception const& ex)
    {
        std::cout << "UNEXPECTED ERROR: " << ex.what() << "\n";
        return UNKNOWN_ERROR;
    }
}
