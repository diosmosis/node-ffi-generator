#include <ffigen/generate/generator/record_map_generator.hpp>
#include <ffigen/generate/generator_factory.hpp>
#include <ffigen/process/code_entity/reference.hpp>
#include <ffigen/process/code_entity/lazy.hpp>
#include <ffigen/utility/logger.hpp>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    static char const* get_record_type(record_entity const& entity)
    {
        return entity.is_union() ? "Union" : "Struct";
    }

    void record_map_generator::define_record_properties(
        record_entity const& entity, std::string const& js_access, std::ostream & os) const
    {
        if (entity.members().empty()) {
            // ref-struct will complain if an empty Struct({}) is used somewhere, so we add a char field
            // as compilers should normally do themselves
            os << "    " << js_access << ".defineProperty(\"__ignore\", 'char');";
            newline(os);
        } else {
            for (auto const& pair : entity.members()) {
                os << "    " << js_access << ".defineProperty(\"" << pair.first << "\", ";

                code_entity member_type = pair.second;
                if (member_type.is_a<lazy_code_entity>()) {
                    member_type = member_type.cast<lazy_code_entity>().get_impl();
                }

                if (member_type.is_anonymous()
                    && member_type.is_a<record_entity>()
                ) {
                    record_entity const& member = member_type.cast<record_entity>();

                    os << get_record_type(member) << "({}));";
                    newline(os);

                    define_record_properties(member, js_access + ".fields." + pair.first + ".type", os);
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
    void record_map_generator::operator()(std::ostream & os) const
    {
        debug() << "record_map_generator::operator(): generating record [indent = " << indent << "]" << std::endl;

        std::string entity_reference = "_library." + entity.name();

        os << entity_reference << " = ";

        char const* type = get_record_type(entity);

        // TODO: have to handle pointers & char * to be string
        os << type << "({});";
        newline(os);
        newline(os);

        os << "_library._preload.push(function () {";
        newline(os);

        define_record_properties(entity, entity_reference, os);

        os << "});";
        newline(os);
        newline(os);

        debug() << "record_map_generator::operator(): finished generating record" << std::endl;
    }
}