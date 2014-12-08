#include <ffigen/generate/generator/typedef_map_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/process/code_entity/record.hpp>
#include <ffigen/process/code_entity/array_entity.hpp>
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

        os << "_library._preload['" << entity.name() << "'] = [";
        output_preload_dependencies(os, entity, false);
        os << "function () {";
        newline(os);

        os << "    _library." << entity.name() << " = ";
        if (!entity.alias_type()) {
            os << "'void';";
        } else if (entity.alias_type().is_anonymous()) {
            factory.make_for(entity.alias_type(), indent)(os);
        } else {
            os << entity.alias_type().ffi_reference() << ";";
        }

        newline(os);

        os << "}];";
        newline(os);
        newline(os);

        debug() << "typedef_map_generator::operator(): finished generating typedef" << std::endl;
    }
}