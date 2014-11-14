#include <ffigen/generate/generator/typedef_map_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <iostream>

namespace ffigen
{
    //! converts
    //!
    //! typedef abc def;
    //!
    //! to
    //!
    //! _library.def = abc;
    void typedef_map_generator::operator()(std::ostream & os) const
    {
        os << "_library." << entity.name() << " = ";
        if (entity.alias_type().is_anonymous())
        {
            factory.make_for(entity.alias_type(), indent + 1)(os);
        }
        else
        {
            os << entity.alias_type().ffi_reference() << ";";
        }

        newline(os); // TODO: empty newlines shouldn't have indents
        newline(os);
    }
}
