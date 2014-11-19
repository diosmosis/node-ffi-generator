#include <ffigen/process/code_entity/lazy.hpp>
#include <ffigen/utility/logger.hpp>

namespace ffigen
{
    using namespace utility::logs;

    void lazy_code_entity::set_internal_pointer() const
    {
        if (!impl)
        {
            code_entity const& actual = symbols.get(fqn);
            if (actual && !actual.is_a<lazy_code_entity>())
            {
                const_cast<code_entity &>(impl) = actual;
            }
            else
            {
                debug() << "lazy_code_entity: couldn't find entity for '" << fqn << "'" << std::endl;
            }
        }
    }
}