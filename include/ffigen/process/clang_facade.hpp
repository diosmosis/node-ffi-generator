#if !defined(NODE_FFI_GENERATOR_FFIGEN_PROCESS_CLANG_FACADE_HPP)
#define NODE_FFI_GENERATOR_FFIGEN_PROCESS_CLANG_FACADE_HPP

#include <ffigen/process/symbol_table.hpp>
#include <list>
#include <string>
#include <unordered_map>
#include <functional>

namespace ffigen
{
    struct clang_facade
    {
        typedef std::function<bool (clang::NamedDecl const&)> symbol_filter_type;

        clang_facade()
            : assume_pragma_once(false)
        {}

        void parse_files(symbol_table & symbols) const;

        std::list<std::string> files_to_process;
        std::list<std::string> include_directories;
        std::unordered_map<std::string, std::string> predefines;
        bool assume_pragma_once;
        std::list<symbol_filter_type> symbol_filters;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_FFIGEN_PROCESS_CLANG_FACADE_HPP)