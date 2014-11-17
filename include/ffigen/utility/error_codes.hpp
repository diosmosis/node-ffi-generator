#if !defined(NODE_FFI_GENERATOR_UTILITY_ERROR_CODES_HPP)
#define NODE_FFI_GENERATOR_UTILITY_ERROR_CODES_HPP

namespace ffigen { namespace error_codes
{
    enum
    {
        UNKNOWN_ERROR = -1,
        SUCCESS = 0,
        MISSING_ARGUMENT = 1,
        INVALID_ARGUMENT = 2,
        HEADER_PARSE_FAIL = 3
    };
}}

#endif // #if !defined(NODE_FFI_GENERATOR_UTILITY_ERROR_CODES_HPP)