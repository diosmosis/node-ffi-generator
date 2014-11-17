#if !defined(NODE_FFI_GENERATOR_UTILITY_EXCEPTIONS_HPP)
#define NODE_FFI_GENERATOR_UTILITY_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace ffigen
{
    struct fatal_error : public std::runtime_error
    {
        fatal_error(std::string const& message, unsigned int error_code)
            : std::runtime_error(message)
            , error_code(error_code)
        {}

        unsigned int const error_code;
    };
}

#endif // #if !defined(NODE_FFI_GENERATOR_UTILITY_EXCEPTIONS_HPP)