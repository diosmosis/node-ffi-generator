#include <ffigen/process/code_entity.hpp>
#include <ffigen/process/code_entity/lazy.hpp>
#include <boost/filesystem.hpp>

namespace ffigen { namespace impl
{
    static code_entity_base const* get_impl(::ffigen::code_entity const& x)
    {
        if (x.is_a<lazy_code_entity>()) {
            return get_impl(static_cast<lazy_code_entity const*>(x.get_impl())->get_impl());
        } else {
            return x.get_impl();
        }
    }

    code_entity_base::code_entity_base(unsigned int type_id, std::string const& name, std::string const& file)
        : base_type(type_id)
        , _name(name)
    {
        if (!file.empty()) {
            _file = boost::filesystem::canonical(file).string();
        }
    }

    bool code_entity_base::is_equal(code_entity_base const* other) const
    {
        if (other
            && other->is_a<lazy_code_entity>()
        ) {
            return other->is_equal(this);
        } else {
            return this == other;
        }
    }
}}

namespace std
{
    size_t hash<ffigen::code_entity>::operator()(ffigen::code_entity const& x) const
    {
        void const* impl = ffigen::impl::get_impl(x);
        return static_cast<base_type const&>(*this)(impl);
    }
}