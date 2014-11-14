#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen { namespace utility { namespace logs
{
    namespace impl
    {
        struct null_stream : public std::ostream
        {
            null_stream() : std::ostream(0) {}
        };
    }

    static levels::value_type _log_level = levels::WARNING;
    static impl::null_stream null_stream;

    void set_level(levels::value_type level)
    {
        _log_level = level;
    }

    std::ostream & do_log(levels::value_type level)
    {
        if (level <= _log_level)
        {
            return std::cout;
        }
        else
        {
            return null_stream;
        }
    }

    std::ostream & error()
    {
        return do_log(levels::ERROR);
    }

    std::ostream & warning()
    {
        return do_log(levels::WARNING);
    }

    std::ostream & info()
    {
        return do_log(levels::INFO);
    }
    
    std::ostream & debug()
    {
        return do_log(levels::DEBUG);
    }
    
    std::ostream & verbose()
    {
        return do_log(levels::VERBOSE);
    }
}}}