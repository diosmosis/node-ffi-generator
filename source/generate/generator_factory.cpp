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
#include <exception>

namespace ffigen
{
    generator generator_factory::make_for(code_entity const& entity, unsigned int indent) const
    {
        if (!entity)
        {
            throw std::runtime_error("generator_factory::make(): null code_entity found.");
        }

        if (entity.is_a<enum_entity>())
        {
            return enum_map_generator(*this, entity.cast<enum_entity>(), indent);
        }
        else if (entity.is_a<function>())
        {
            return function_map_generator(*this, entity.cast<function>(), indent);
        }
        else if (entity.is_a<struct_entity>())
        {
            return struct_map_generator(*this, entity.cast<struct_entity>(), indent);
        }
        else if (entity.is_a<union_entity>())
        {
            return union_map_generator(*this, entity.cast<union_entity>(), indent);
        }
        else if (entity.is_a<typedef_entity>())
        {
            return typedef_map_generator(*this, entity.cast<typedef_entity>(), indent);
        }
        else
        {
            throw std::runtime_error(std::string("generator_factory::make(): Invalid code entity found, name = ")
                + entity.name() + ", file = " + entity.file());
        }
    }
}
