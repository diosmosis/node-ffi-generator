#include <ffigen/generate_node_ffi_interface.hpp>
#include <ffigen/utility/error_codes.hpp>
#include <ffigen/utility/exceptions.hpp>
#include <ffigen/utility/logger.hpp>
#include <argumentative/argumentative.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <list>
#include <stdexcept>
#include <iostream>

namespace fs = boost::filesystem;

void recursive_glob_for_headers(std::list<std::string> & files, fs::path const& path)
{
    fs::recursive_directory_iterator i(path), end;
    for (; i != end; ++i) {
        fs::path const& path(i->path());

        std::string extension = path.extension().string();
        if (extension == ".h" || extension == ".hpp" || extension == ".hxx") {
            files.push_back(path.string());
        }
    }
}

int main(int argc, char ** argv)
{
    using namespace ffigen::utility::logs;
    using namespace ffigen::error_codes;
    using ffigen::fatal_error;
    using namespace argumentative::dsel;

    set_level(levels::INFO);

    bool help_requested = false;
    std::list<std::string> files_to_process;
    std::string dest, src_root;
    std::list<std::string> include_directories;

    argumentative::cli cli("node-ffi-generator");

    cli += option("help")([&help_requested] (std::string const&) {
        std::cout << "Generates node-ffi JS bindings using one or more C header files. Embeds clang's parser to do so.\n\n";
        std::cout << "Usage: node-ffi-generator --file=path/to/headers --dest=path/to/jsfiles/destination --src-root=path/headers/root/dir\n\n";
        std::cout << "Options:\n";
        std::cout << "  --file=path/to/file      C header file or directory with header files to process. If a directory,\n";
        std::cout << "                           it is searched recursively for .h, .hpp and .hxx files.\n";
        std::cout << "  --dest=path/to/dir       The destination directory for the mapping files.\n";
        std::cout << "  --src-root=path/to/dir   The root directory of the C library that's being mapped. Helps to determine\n";
        std::cout << "                           the destination's directory structure.\n";
        std::cout << "  --include=path/to/dir    A directory to use when searching for headers.\n";

        help_requested = true;
    });

    cli += option("file", "", option_value::required)([&files_to_process] (std::string const& value) {
        // TODO: should support glob expressions
        fs::path path(value);
        if (fs::is_directory(path)) {
            recursive_glob_for_headers(files_to_process, path);
        } else if (fs::is_regular_file(path)) {
            files_to_process.push_back(value);
        } else {
            throw fatal_error(std::string("'") + value + "' supplied in --file option does not exist.", INVALID_ARGUMENT);
        }

        debug() << "Processed --file argument '" << value << "'" << std::endl;
    });

    cli += option("dest", "", option_value::required)([&dest] (std::string const& value) {
        if (!fs::is_directory(fs::path(value))) {
            throw fatal_error(std::string("Destination '") + value + "' is not a valid directory.", INVALID_ARGUMENT);
        }

        dest = value;

        debug() << "Processed --dest argument '" << dest << "'" << std::endl;
    });

    cli += option("src-root", "", option_value::required)([&src_root, &include_directories] (std::string const& value) {
        if (!fs::is_directory(fs::path(value))) {
            throw fatal_error(std::string("ERROR: Source root directory '") + value + "' is not a valid directory.", INVALID_ARGUMENT);
        }

        src_root = value;

        include_directories.push_back(src_root);

        debug() << "Processed --src-root argument '" << src_root << "'" << std::endl;
    });

    cli += option("log-level", "", option_value::required)([] (std::string const& value) {
        if (value == "ERROR") {
            set_level(levels::ERROR);
        } else if (value == "WARNING") {
            set_level(levels::WARNING);
        } else if (value == "INFO") {
            set_level(levels::INFO);
        } else if (value == "DEBUG") {
            set_level(levels::DEBUG);
        } else if (value == "VERBOSE") {
            set_level(levels::VERBOSE);
        } else {
            throw fatal_error(std::string("ERROR: Invalid log level supplied in --log-level option '") + value + ";.",
                INVALID_ARGUMENT);
        }
    });

    cli += option("include", "", option_value::required)([&include_directories] (std::string const& value) {
        if (!fs::is_directory(fs::path(value))) {
            throw fatal_error(std::string("ERROR: Include directory '") + value + "' is not a valid directory.",
                INVALID_ARGUMENT);
        }

        include_directories.push_back(value);
    });

    cli += option("define", "D", option_value::required)([] (std::string const& value) {
        // TODO
    });

    try {
        cli.parse(argv, argv + argc);

        if (help_requested) { // TODO: should be a way to exit w/ success from closure...
            return 0;
        }

        if (files_to_process.empty()) {
            std::cout << "ERROR: Required option 'file' not supplied.\n";
            return MISSING_ARGUMENT;
        }

        if (dest.empty()) {
            std::cout << "ERROR: Required option 'dest' not supplied.\n";
            return MISSING_ARGUMENT;
        }

        if (src_root.empty()) {
            std::cout << "ERROR: Required option 'src-root' not supplied.\n";
            return MISSING_ARGUMENT;
        }

        ffigen::generate_node_ffi_interface(files_to_process, src_root, dest, include_directories);
    } catch (ffigen::fatal_error const& ex) {
        std::cout << "ERROR: " << ex.what() << std::endl;
        return ex.error_code;
    } catch (std::exception const& ex) {
        std::cout << "UNEXPECTED ERROR: " << ex.what() << std::endl;
        return UNKNOWN_ERROR;
    }
}