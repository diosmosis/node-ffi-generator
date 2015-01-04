#include <ffigen/generate/generator.hpp>
#include <ffigen/process/symbol/reference_symbol.hpp>
#include <ffigen/process/symbol/typedef_symbol.hpp>
#include <ffigen/process/symbol/array_symbol.hpp>
#include <ffigen/process/symbol/record_symbol.hpp>
#include <ffigen/process/symbol/reference_symbol.hpp>
#include <ffigen/process/symbol/fundamental_type_symbol.hpp>
#include <ffigen/process/symbol.hpp>
#include <iostream>

namespace ffigen { namespace impl
{
    typedef ::ffigen::symbol symbol_erased;

    symbol_erased const* generator_base::get_associated_type(symbol_erased const& entity) const
    {
        symbol_erased const* real = &entity;

        if (real->is_a<array_symbol>()) {
            real = &entity.cast<array_symbol>().element_type();
        } else if (real->is_a<reference_symbol>()) {
            real = &entity.cast<reference_symbol>().pointee();
        }

        if (real->is_a<fundamental_type_symbol>()) {
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

    void generator_base::output_preload_dependencies(std::ostream & os, symbol_erased const& entity, bool include_records) const
    {
        for (symbol_erased const* dependent : entity.dependents()) {
            symbol_erased const* type = get_associated_type(*dependent);
            if (!type
                || !*type
            ) {
                continue;
            }

            if (type->is_anonymous()) {
                output_preload_dependencies(os, *type);
            } else {
                if (!include_records
                    && type->is_a<record_symbol>()
                ) {
                    continue;
                }

                os << "'" << type->name() << "', ";
            }
        }
    }
}}