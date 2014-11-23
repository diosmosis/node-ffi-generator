#include <ffigen/generate/generator.hpp>
#include <ffigen/generate/generator/enum_map_generator.hpp>
#include <ffigen/generate/generator/function_map_generator.hpp>
#include <ffigen/generate/generator/struct_map_generator.hpp>
#include <ffigen/generate/generator/typedef_map_generator.hpp>
#include <ffigen/generate/generator/union_map_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/process/code_entity/enum.hpp>
#include <ffigen/process/code_entity/function.hpp>
#include <ffigen/process/code_entity/struct.hpp>
#include <ffigen/process/code_entity/typedef.hpp>
#include <ffigen/process/code_entity/union.hpp>
#include <ffigen/process/code_entity/lazy.hpp>
#include <ffigen/utility/logger.hpp>
#include <exception>

namespace ffigen
{
    using namespace utility::logs;

    generator generator_factory::make_for(code_entity entity, unsigned int indent) const
    {
        if (!entity)
        {
            throw std::runtime_error("generator_factory::make(): null code_entity found.");
        }

        if (entity.is_a<lazy_code_entity>())
        {
            entity = entity.cast<lazy_code_entity>().get_impl();
        }

        if (entity.is_a<enum_entity>())
        {
            debug() << "creating enum generator for '" << entity.name() << "'" << std::endl;

            return enum_map_generator(*this, entity.cast<enum_entity>(), indent);
        }
        else if (entity.is_a<function>())
        {
            debug() << "creating function generator for '" << entity.name() << "'" << std::endl;

            return function_map_generator(*this, entity.cast<function>(), indent);
        }
        else if (entity.is_a<struct_entity>())
        {
            debug() << "creating struct generator for '" << entity.name() << "'" << std::endl;

            return struct_map_generator(*this, entity.cast<struct_entity>(), indent);
        }
        else if (entity.is_a<union_entity>())
        {
            debug() << "creating union generator for '" << entity.name() << "'" << std::endl;

            return union_map_generator(*this, entity.cast<union_entity>(), indent);
        }
        else if (entity.is_a<typedef_entity>())
        {
            debug() << "creating typedef generator for '" << entity.name() << "'" << std::endl;

            return typedef_map_generator(*this, entity.cast<typedef_entity>(), indent);
        }
        else
        {
            throw std::runtime_error(std::string("generator_factory::make(): Invalid code entity found, type = ")
                + entity.get_type_name() + ", name = " + entity.name() + ", file = " + entity.file());
        }
    }

    bool generator_factory::can_generate_for(code_entity entity) const
    {
        if (!entity) {
            return false;
        }

        if (entity.is_a<lazy_code_entity>())
        {
            entity = entity.cast<lazy_code_entity>().get_impl();
        }

        return entity.is_a<enum_entity>()
            || entity.is_a<function>()
            || entity.is_a<struct_entity>()
            || entity.is_a<union_entity>()
            || entity.is_a<typedef_entity>();
    }
}
