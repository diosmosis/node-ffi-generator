#include <ffigen/process/symbol/record_symbol.hpp>
#include <ffigen/process/symbol/typedef_symbol.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/process/symbol/reference_symbol.hpp>
#include <ffigen/process/symbol/typedef_symbol.hpp>
#include <ffigen/process/symbol/array_symbol.hpp>
#include <ffigen/utility/logger.hpp>
#include <unordered_set>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    static symbol const* get_typedef_record(symbol const& entity)
    {
        if (entity.is_a<typedef_symbol>()) {
            symbol const& alias_type = entity.cast<typedef_symbol>().alias_type();
            return get_typedef_record(alias_type);
        } else if (entity.is_a<record_symbol>()) {
            return &entity;
        } else {
            return nullptr;
        }
    }

    void record_symbol::fill_dependents() const
    {
        for (auto const& pair : _members) {
            _dependents.push_back(&pair.second);

            symbol const* typedef_record = get_typedef_record(pair.second);
            if (typedef_record) {
                dependent_records.push_back(*typedef_record);
                _dependents.push_back(&dependent_records.back());
            }
        }
    }

    // record_symbol::generator
    static std::unordered_set<std::string> reserved_property_names;

    std::unordered_set<std::string> const& get_reserved_property_names()
    {
        if (reserved_property_names.empty()) {
            reserved_property_names.insert("constructor");
        }
        return reserved_property_names;
    }

    static char const* get_record_type(record_symbol const& entity)
    {
        return entity.is_union() ? "Union" : "Struct";
    }

    std::string get_property_name(std::string const& name)
    {
        std::unordered_set<std::string> const& names = get_reserved_property_names();

        // if a property name is already used in JS (like 'constructor'), append two underscores to avoid errors
        auto i = names.find(name);
        if (i != names.end()) {
            return name + "__";
        } else {
            return name;
        }
    }

    // TODO: instead of having this code where we look for the right type of dependent entities,
    //       symbol classes should add the correct dependencies to the _dependents list, then we can
    //       just iterate over it. sort of done for record w/ record.cpp. should be expanded to all types.
    //       this way we just depend on the data structure in the generator.
    symbol const* record_symbol::generator::get_associated_type(symbol const& entity) const
    {
        // if a record or typedef is stored by value in a record, the record must depend
        // on the underlying records
        if (entity.is_a<record_symbol>()) {
            return &entity;
        }

        return impl::generator_base::get_associated_type(entity);
    }

    void record_symbol::generator::define_record_properties(
        record_symbol const& entity, std::string const& js_access, std::ostream & os) const
    {
        if (entity.members().empty()) {
            // ref-struct will complain if an empty Struct({}) is used somewhere, so we add a char field
            // as compilers should normally do themselves
            os << "    " << js_access << ".defineProperty(\"__ignore\", 'char');";
            newline(os);
        } else {
            for (auto const& pair : entity.members()) {
                os << "    " << js_access << ".defineProperty(\"" << get_property_name(pair.first) << "\", ";

                symbol member_type = pair.second;

                if (member_type.is_anonymous()
                    && member_type.is_a<record_symbol>()
                ) {
                    record_symbol const& member = member_type.cast<record_symbol>();

                    os << "(function () {";

                    ++indent;
                    newline(os);

                    os << "    var temp = " << get_record_type(member) << "({});";
                    newline(os);

                    define_record_properties(member, "temp", os);

                    --indent;

                    os << "    return temp;";
                    newline(os);

                    os << "    })());";
                    newline(os);
                } else {
                    os << member_type.ffi_reference() << ");";
                    newline(os);
                }
            }
        }
    }

    //! converts TODO: comments are no longer accurate.
    //!
    //! struct my_struct { int a; int b; struct {int c;} d; };
    //!
    //! to
    //!
    //! _library.my_struct = Struct({
    //!     a: _library.int,
    //!     b: _library.int,
    //!     d: Struct({
    //!         c: _library.int
    //!     })
    //! });
    void record_symbol::generator::operator()(std::ostream & os) const
    {
        debug() << "record_symbol::generator::operator(): generating record_symbol [indent = " << indent << "]" << std::endl;

        std::string entity_reference = "_library." + entity.name();

        os << entity_reference << " = ";

        char const* type = get_record_type(entity);

        // TODO: have to handle pointers & char * to be string
        os << type << "({});";
        newline(os);

        // hack to get around assertion in node-ffi ref libs. if size is 0, then ref libs will throw
        // when using the type in functions/refType calls/etc. here we set it to 1 to make sure no
        // exceptions are thrown, then later when defining, we reset it to 0 so there are no issues
        // w/ struct/union buffer management.
        os << entity_reference << ".size = 1;";
        newline(os);
        newline(os);

        os << "_library._preload['" << entity.name() << "'] = [";
        output_preload_dependencies(os, entity);
        os << "function () {";
        newline(os);

        os << "    " << entity_reference << ".size = 0;";
        newline(os);

        define_record_properties(entity, entity_reference, os);

        os << "}];";
        newline(os);
        newline(os);

        debug() << "record_symbol::generator::operator(): finished generating record_symbol" << std::endl;
    }
}