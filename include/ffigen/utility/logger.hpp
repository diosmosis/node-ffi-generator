#if !defined(NODE_FFI_GENERATOR_UTILITY_LOGGER_HPP)
#define NODE_FFI_GENERATOR_UTILITY_LOGGER_HPP

#include <string>
#include <iosfwd>

namespace ffigen { namespace utility { namespace logs
{
    struct levels
    {
        enum value_type
        {
            ERROR,
            WARNING,
            INFO,
            DEBUG,
            VERBOSE
        };
    };

    std::ostream & error();
    std::ostream & warning();
    std::ostream & info();
    std::ostream & debug();
    std::ostream & verbose();

    void set_level(levels::value_type);
}}}

#endif // #if !defined(NODE_FFI_GENERATOR_UTILITY_LOGGER_HPP)