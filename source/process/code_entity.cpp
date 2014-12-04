#include <ffigen/process/code_entity.hpp>
#include <boost/filesystem.hpp>

namespace ffigen { namespace impl
{
    code_entity_base::code_entity_base(unsigned int type_id, std::string const& name, std::string const& file)
        : base_type(type_id)
        , _name(name)
    {
        if (!file.empty()) {
            _file = boost::filesystem::canonical(file).string();
        }
    }
}}