#include <ffigen/generate/generator/library_load_generator.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    //! creates a function that loads the shared object w/ function mapping, eg,
    //!
    //! require('whatever/abc');
    //! require('whatever/def');
    //!
    //! exports = function (path) {
    //!     return ffi.Library({
    //!         function1: _library.function1,
    //!         function2: _library.function2
    //!         // ...
    //!     });
    //! };
    void library_load_generator::operator()(std::ostream & os) const
    {
        debug() << "library_load_generator::operator(): generating library entry" << std::endl;

        os << "exports = function (path) {";
        newline(os);

        os << "    var _library = this;";
        newline(os);

        os << "    return ffi.Library({";
        newline(os);

        bool is_first = true;
        for (code_entity const& entity : symbols.entities())
        {
            if (entity.is_a<function>())
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    os << ",";
                    newline(os);
                }
                os << "        " << entity.name() << ": _library." << entity.name();
            }
        }

        newline(os);
        os << "    });";

        newline(os);
        os << "};";

        newline(os);
        newline(os);

        for (std::string const& module : modules)
        {
            os << "require('./" << module << "');";
            newline(os);
        }

        debug() << "library_load_generator::operator(): finished generating library entry" << std::endl;
    }
}
