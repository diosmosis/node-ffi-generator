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

        {
            os << "var exports = module.exports = function (path) {";
            newline(os);

            os << "    var _library = exports;";
            newline(os);

            os << "    for (var key in _library._preload) {";
            newline(os);

            os << "        if (!_library._preload.hasOwnProperty(key)) {";
            newline(os);

            os << "            continue;";
            newline(os);

            os << "        }";
            newline(os);

            os << "        invoke_preload_function(key);";
            newline(os);

            os << "    }";
            newline(os);

            os << "    return ffi.Library(path, _library._functions);";
            newline(os);

            os << "};";

            newline(os);
            newline(os);
        }

        {
            os << "exports._functions = {};";
            newline(os);

            os << "exports._preload = [];";
            newline(os);
            newline(os);
        }

        { // avoiding ref-array bug for typedefs of RefArray
            os << "// avoid prototype chain bug in ref-array that is triggered when an array is used in a struct";
            newline(os);

            os << "exports.__RefArray = function () {";
            newline(os);

            os << "    var result = RefArray.apply(null, arguments);";
            newline(os);

            os << "    result.toString = function () { return this.type.toString() + '[]'; };";
            newline(os);

            os << "    return result;";
            newline(os);

            os << "};";
            newline(os);
            newline(os);
        }

        {
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

            newline(os);

            os << "};";
            newline(os);
            newline(os);
        }

        {
            os << "exports.loadAllBindings = function () {";
            newline(os);

            for (std::string const& module : modules)
            {
                os << "    require('./" << module << "');";
                newline(os);
            }

            os << "};";
            newline(os);
            newline(os);
        }

        {
            os << "function invoke_preload_function(key) {";
            newline(os);
            
            os << "    var _library = exports;";
            newline(os);

            os << "    var entry = _library._preload[key];";
            newline(os);

            os << "    delete _library._preload[key];";
            newline(os);
            newline(os);

            os << "    if (entry) {";
            newline(os);

            os << "        var func = entry.pop();";
            newline(os);
            newline(os);

            os << "        // execute dependent preloads";
            newline(os);

            os << "        for (var i = 0; i != entry.length; ++i) {";
            newline(os);

            os << "            invoke_preload_function(entry[i]);";
            newline(os);

            os << "        }";
            newline(os);
            newline(os);

            os << "        func();";
            newline(os);

            os << "    }";
            newline(os);

            os << "}";
            newline(os);
        }

        debug() << "library_load_generator::operator(): finished generating library entry" << std::endl;
    }
}