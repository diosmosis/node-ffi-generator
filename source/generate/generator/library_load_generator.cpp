#include <ffigen/generate/generator/library_load_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
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

        os << "    return ffi.Library(_library._functions);";
        newline(os);

        newline(os);
        os << "};";

        newline(os);
        newline(os);

        os << "exports._functions = [];";
        newline(os);
        newline(os);

        os << "exports.loadDependentSymbols = function () {";
        newline(os);

        os << "    var _library = exports;";
        newline(os);

        os << "    ";
        symbols.dfs(external_symbols, "", [&os, this] (code_entity const& entity) {
            if (factory.can_generate_for(entity)) {
                factory.make_for(entity, indent + 1)(os);
            }
        });

        os << "};";
        newline(os);
        newline(os);

        os << "exports.loadAllBindings = function () {";
        newline(os);

        for (std::string const& module : modules)
        {
            os << "    require('./" << module << "');";
            newline(os);
        }

        os << "};";
        newline(os);

        debug() << "library_load_generator::operator(): finished generating library entry" << std::endl;
    }
}