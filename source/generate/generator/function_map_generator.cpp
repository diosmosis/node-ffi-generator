#include <ffigen/generate/generator/function_map_generator.hpp>
#include <ffigen/utility/logger.hpp>
#include <ffigen/utility/exceptions.hpp>
#include <ffigen/utility/error_codes.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    //! converts
    //!
    //! int my_function(params...)
    //!
    //! to
    //!
    //! _library.my_function = [_library.int, [params...]];
    void function_map_generator::operator()(std::ostream & os) const
    {
        debug() << "function_map_generator::operator(): generating function" << std::endl;

        if (entity.is_variadic()) {
            error() << "Variadic functions not supported yet (trying to generate'"
                    << entity.name() << "')" << std::endl;
            return;
        }

        os << "_library." << entity.name() << " = [";
        os << entity.return_type().ffi_reference();
        os << ", [";

        bool is_first = true;
        for (code_entity const& parameter : entity.argument_types())
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                os << ", ";
            }
            os << parameter.ffi_reference();
        }

        os << "]];";
        newline(os);

        os << "_library._functions['" << entity.name() << "'] = _library." << entity.name() << ";";
        newline(os);
        newline(os);

        debug() << "function_map_generator::operator(): finished generating function" << std::endl;
    }
}

// TODO: there are special keywords generated by node-ffi-generator. if a type is named after it, we have to skip mapping it.
//       special keywords are 'exports', 'module', 'require', '_library', ...
