#include <ffigen/generate/generator/typedef_map_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    //! converts
    //!
    //! typedef abc def;
    //!
    //! to
    //!
    //! _library.def = abc;
    void typedef_map_generator::operator()(std::ostream & os) const
    {
        debug() << "typedef_map_generator::operator(): generating typedef" << std::endl;

        os << "_library." << entity.name() << " = ";
        if (!entity.alias_type()) {
            os << "'void';";
        } else if (entity.alias_type().is_anonymous()) {
            factory.make_for(entity.alias_type(), indent)(os);
        } else {
            os << entity.alias_type().ffi_reference() << ";";
        }

        newline(os); // TODO: empty newlines shouldn't have indents
        newline(os);

        debug() << "typedef_map_generator::operator(): finished generating typedef" << std::endl;
    }
}
