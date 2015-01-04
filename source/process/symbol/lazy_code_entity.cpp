#include <ffigen/process/symbol/lazy.hpp>
#include <ffigen/utility/logger.hpp>

namespace ffigen
{
    using namespace utility::logs;

    void lazy_symbol::set_internal_pointer() const
    {
        if (!impl)
        {
            symbol const* actual = symbols.find(fqn);
            if (actual
                && !actual->is_a<lazy_symbol>()
                && *actual
            ) {
                const_cast<symbol &>(impl) = *actual;
            }
        }
    }

    void lazy_symbol::check_internal_pointer() const
    {
        if (!impl)
        {
            debug() << "lazy_symbol: couldn't find entity for '" << fqn << "'" << std::endl;
        }
    }
}