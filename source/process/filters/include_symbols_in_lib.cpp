#include <ffigen/process/filters/include_symbols_in_lib.hpp>
#include <ffigen/process/symbol.hpp>
#include <ffigen/process/symbol/function_symbol.hpp>
#include <ffigen/utility/error_codes.hpp>
#include <ffigen/utility/exceptions.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace ffigen
{
    namespace fs = boost::filesystem;

    include_symbols_in_lib::include_symbols_in_lib(std::string const& path)
        : lib(new annex::shared_library(path))
    {
        if (!fs::exists(fs::path(path))) {
            throw fatal_error("File '" + path + "' does not exist!", error_codes::INVALID_ARGUMENT);
        }

        if (!lib) {
            throw fatal_error("Unable to load shared object '" + path + "'.", error_codes::INVALID_ARGUMENT);
        }
    }

    bool include_symbols_in_lib::operator()(clang::NamedDecl const& node) const
    {
        if (llvm::dyn_cast<clang::FunctionDecl const>(&node)) {
            if (!lib->get(node.getNameAsString().c_str())) {
                return false;
            }
        }

        return true;
    }
}