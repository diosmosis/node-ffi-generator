#include <ffigen/generate/generator/union_map_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <iostream>

namespace ffigen
{
    //! converts
    //!
    //! union whatever
    //! {
    //!     int a;
    //!     int b;
    //!     struct {
    //!         int c;
    //!     } d;
    //! }
    //!
    //! to
    //!
    //! _library.whatever = Untion({
    //!     a: 'int',
    //!     b: 'int'
    //!     d: Struct({
    //!         c: 'int'
    //!     })
    //! })
    void union_map_generator::operator()(std::ostream & os) const // TODO: code redundancy w/ struct map generator. could just add a is_union flag.
    {
        if (!entity.is_anonymous())
        {
            os << "_library." << entity.name() << " = ";
        }

        os << "Union({";
        newline(os);

        bool is_first = true;
        for (auto const& pair : entity.variants())
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
