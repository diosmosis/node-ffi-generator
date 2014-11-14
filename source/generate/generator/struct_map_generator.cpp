#include <ffigen/generate/generator/struct_map_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <iostream>

namespace ffigen
{
    //! converts
    //!
    //! struct my_struct { int a; int b; struct {int c;} d; };
    //!
    //! to
    //!
    //! _library.my_struct = Struct({
    //!     a: _library.int,
    //!     b: _library.int,
    //!     d: Struct({
    //!         c: _library.int
    //!     })
    //! });
    void struct_map_generator::operator()(std::ostream & os) const
    {
        if (!entity.is_anonymous())
        {
            os << "_library." << entity.name() << " = ";
        }

        // TODO: have to handle pointers & char * to be string
        os << "Struct({";
        newline(os);

        bool is_first = true;
        for (auto const& pair : entity.members())
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
            os << "    " << pair.first << ": ";

            if (pair.second->is_anonymous())
            {
                factory.make_for(*pair.second, indent + 1)(os);
            }
            else
            {
                os << pair.second->ffi_reference();
            }
        }

        newline(os);
        os << "});";

        if (!entity.is_anonymous())
        {
            newline(os);
            newline(os);
        }
    }
}
