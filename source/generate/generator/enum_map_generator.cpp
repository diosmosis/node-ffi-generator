#include <ffigen/generate/generator/enum_map_generator.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    //! converts
    //!
    //! enum my_enum { A = 1, B = 2, C = 3 }
    //!
    //! to
    //!
    //! _library.my_enum = {A: 1, B: 2, C: 3};
    void enum_map_generator::operator()(std::ostream & os) const
    {
        debug() << "enum_map_generator::operator(): generating enum" << std::endl;

        os << "_library." << entity.name() << " = {";
        newline(os);

        bool isFirst = true;
        for (auto const& pair : entity.values()) {
            if (isFirst)
            {
                isFirst = false;
            }
            else
            {
                os << ",";
                newline(os);
            }

            os << "    " << pair.first << ": " << pair.second;
        }

        newline(os);
        os << "};";

        newline(os);
        newline(os);

        debug() << "enum_map_generator::operator(): finished generating enum" << std::endl;
    }
}
