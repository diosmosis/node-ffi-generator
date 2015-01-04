#include <ffigen/process/code_entity/enum.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    // enum_entity
    enum_entity::enum_entity(std::string const& name, std::string const& file, values_map_type const& values)
        : base_type(name, file)
        , _values(values)
    {}

    // enum_entity::generator

    //! converts
    //!
    //! enum my_enum { A = 1, B = 2, C = 3 }
    //!
    //! to
    //!
    //! _library.my_enum = {A: 1, B: 2, C: 3};
    void enum_entity::generator::operator()(std::ostream & os) const
    {
        debug() << "enum::generator::operator(): generating enum" << std::endl;

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

        debug() << "enum::generator::operator(): finished generating enum" << std::endl;
    }
}