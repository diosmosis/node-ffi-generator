#if !defined(NODE_FFI_GENERATOR_FFIGEN_PROCESS_CLANG_FACADE_HPP)
#define NODE_FFI_GENERATOR_FFIGEN_PROCESS_CLANG_FACADE_HPP

#include <list>
#include <string>
#include <unordered_map>

namespace ffigen
{
    struct clang_facade
    {
        clang_facade()
            : assume_pragma_once(false)
        {}

        std::list<std::string> files_to_process;
        std::list<std::string> include_directories;
        std::unordered_map<std::string, std::string> predefines;
        bool assume_pragma_once;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_FFIGEN_PROCESS_CLANG_FACADE_HPP)