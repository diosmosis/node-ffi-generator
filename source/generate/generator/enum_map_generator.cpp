#include <ffigen/generate/generator/enum_map_generator.hpp>
#include <iostream>

namespace ffigen
{
    //! converts
    //!
    //! enum my_enum { A = 1, B = 2, C = 3 }
    //!
    //! to
    //!
    //! _library.my_enum = {A: 1, B: 2, C: 3};
    void enum_map_generator::operator()(std::ostream & os) const
    {
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
    }
}
