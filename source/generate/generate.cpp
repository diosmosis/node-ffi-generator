#include <ffigen/generate/generator.hpp>
#include <iostream>

namespace ffigen { namespace impl
{
    void generator_base::newline(std::ostream & os) const
    {
        os << "\n";
        for (unsigned int i = 0; i != indent; ++i)
        {
            os << "    ";
        }
        os << std::flush;
    }
}}
