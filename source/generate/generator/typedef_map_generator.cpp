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

        bool in_preload = false;
        if (entity.alias_type().is_a<array_entity>()
                   && entity.alias_type().cast<array_entity>().element_type().is_a<record_entity>()
        ) {
            in_preload = true;

            // records are defined lazily, so in this case where it's a typedef of an array of a struct,
            // it must be done in a _preload function. this will cause problems if the array is used outside
            // of a struct (ie, as a function argument)
            os << "_library._preload.push(function () {";
            newline(os);

            os << "    ";
        }

        os << "_library." << entity.name() << " = ";
        if (!entity.alias_type()) {
            os << "'void';";
        } else if (entity.alias_type().is_anonymous()) {
            factory.make_for(entity.alias_type(), indent)(os);
        } else {
            os << entity.alias_type().ffi_reference() << ";";
        }

        newline(os);

        if (in_preload) {
            os << "});";
            newline(os);
        }

        newline(os); // TODO: empty newlines shouldn't have indents

        debug() << "typedef_map_generator::operator(): finished generating typedef" << std::endl;
    }
}