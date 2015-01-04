#include <ffigen/generate/generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/process/symbol/enum_symbol.hpp>
#include <ffigen/process/symbol/function_symbol.hpp>
#include <ffigen/process/symbol/record_symbol.hpp>
#include <ffigen/process/symbol/typedef_symbol.hpp>
#include <ffigen/utility/logger.hpp>
#include <exception>

namespace ffigen
{
    using namespace utility::logs;

    generator generator_factory::make_for(symbol const& entity, unsigned int indent) const
    {
        if (!entity) {
            throw std::runtime_error("generator_factory::make(): null symbol found.");
        }

        if (entity.is_a<enum_symbol>()) {
            debug() << "creating enum generator for '" << entity.name() << "'" << std::endl;

            return enum_symbol::generator(*this, entity.cast<enum_symbol>(), indent);
        } else if (entity.is_a<function_symbol>()) {
            debug() << "creating function generator for '" << entity.name() << "'" << std::endl;

            return function_symbol::generator(*this, entity.cast<function_symbol>(), indent);
        } else if (entity.is_a<record_symbol>()) {
            debug() << "creating record_symbol.hpp generator for '" << entity.name() << "'" << std::endl;

            return record_symbol::generator(*this, entity.cast<record_symbol>(), indent);
        } else if (entity.is_a<typedef_symbol>()) {
            debug() << "creating typedef generator for '" << entity.name() << "'" << std::endl;

            return typedef_symbol::generator(*this, entity.cast<typedef_symbol>(), indent);
        } else {
            throw std::runtime_error(std::string("generator_factory::make(): Invalid code entity found, type = ")
                + entity.get_type_name() + ", name = " + entity.name() + ", file = " + entity.file());
        }
    }

    bool generator_factory::can_generate_for(symbol const& entity) const
    {
        if (!entity) {
            return false;
        }

        return entity.is_a<enum_symbol>()
            || entity.is_a<function_symbol>()
            || entity.is_a<record_symbol>()
            || entity.is_a<typedef_symbol>();
    }
}
