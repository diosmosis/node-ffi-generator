#include <ffigen/process/symbol.hpp>
#include <ffigen/process/symbol/lazy_symbol.hpp>
#include <boost/filesystem.hpp>

namespace ffigen { namespace impl
{
    static symbol_base const* get_impl(::ffigen::symbol const& x)
    {
        if (x.is_a<lazy_symbol>()) {
            return get_impl(static_cast<lazy_symbol const*>(x.get_impl())->get_impl());
        } else {
            return x.get_impl();
        }
    }

    symbol_base::symbol_base(unsigned int type_id, std::string const& name, std::string const& file)
        : base_type(type_id)
        , _name(name)
    {
        if (!file.empty()) {
            _file = boost::filesystem::canonical(file).string();
        }
    }

    bool symbol_base::is_equal(symbol_base const* other) const
    {
        if (other
            && other->is_a<lazy_symbol>()
        ) {
            return other->is_equal(this);
        } else {
            return this == other;
        }
    }
}}

namespace std
{
    size_t hash<ffigen::symbol>::operator()(ffigen::symbol const& x) const
    {
        void const* impl = ffigen::impl::get_impl(x);
        return static_cast<base_type const&>(*this)(impl);
    }
}