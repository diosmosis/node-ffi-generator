#include <ffigen/generate/generator.hpp>
#include <ffigen/process/code_entity/reference.hpp>
#include <ffigen/process/code_entity/lazy.hpp>
#include <ffigen/process/code_entity/typedef.hpp>
#include <ffigen/process/code_entity/array_entity.hpp>
#include <ffigen/process/code_entity/record.hpp>
#include <ffigen/process/code_entity/reference.hpp>
#include <ffigen/process/code_entity/fundamental_type.hpp>
#include <ffigen/process/code_entity.hpp>
#include <iostream>

namespace ffigen { namespace impl
{
    typedef ::ffigen::code_entity code_entity_erased;

    code_entity_erased const* generator_base::get_associated_type(code_entity_erased const& entity) const
    {
        code_entity_erased const* real = &entity;
        if (entity.is_a<lazy_code_entity>()) {
            real = &entity.cast<lazy_code_entity>().get_impl();
        }

        if (real->is_a<array_entity>()) {
            real = &entity.cast<array_entity>().element_type();
        } else if (real->is_a<reference_entity>()) {
            real = &entity.cast<reference_entity>().pointee();
        }

        if (real->is_a<fundamental_type_entity>()) {
            real = nullptr;
        }

        return real;
    }

    void generator_base::newline(std::ostream & os, unsigned int indent) const
    {
        os << "\n";
        for (unsigned int i = 0; i != indent; ++i)
        {
            os << "    ";
        }
        os << std::flush;
    }

    void generator_base::newline(std::ostream & os) const
    {
        this->newline(os, indent);
    }

    void generator_base::output_preload_dependencies(std::ostream & os, code_entity_erased const& entity, bool include_records) const
    {
        for (code_entity_erased const* dependent : entity.dependents()) {
            code_entity_erased const* type = get_associated_type(*dependent);
            if (!type
                || !*type
            ) {
                continue;
            }

            if (type->is_anonymous()) {
                output_preload_dependencies(os, *type);
            } else {
                if (!include_records
                    && type->is_a<record_entity>()
                ) {
                    continue;
                }

                os << "'" << type->name() << "', ";
            }
        }
    }
}}