#include <llvm/Config/llvm-config.h>
#include <ffigen/process/code_entity_factory.hpp>
#include <ffigen/process/code_entity/enum.hpp>
#include <ffigen/process/code_entity/function.hpp>
#include <ffigen/process/code_entity/struct.hpp>
#include <ffigen/process/code_entity/typedef.hpp>
#include <ffigen/process/code_entity/union.hpp>
#include <ffigen/process/code_entity/reference.hpp>
#include <ffigen/process/code_entity/fundamental_type.hpp>
#include <ffigen/utility/logger.hpp>
#include <stdexcept>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    code_entity code_entity_factory::make(clang::NamedDecl const& node) const
    {
        debug() << "code_entity_factory::make('" << node.getNameAsString() << "')" << std::endl;

        clang::PresumedLoc loc = source_manager.getPresumedLoc(node.getLocation());

        std::string name = node.getNameAsString();
        std::string file = loc.getFilename();

        debug() << "code_entity_factory::make(): [ name = '" << name << "', file = '" << file << "' ]" << std::endl;

        if (clang::EnumDecl const* enum_node = llvm::dyn_cast<clang::EnumDecl const>(&node))
        {
            return make_enum(*enum_node, name, file);
        }

        if (clang::FunctionDecl const* function_node = llvm::dyn_cast<clang::FunctionDecl const>(&node))
        {
            return make_function(*function_node, name, file);
        }

        if (clang::TypedefNameDecl const* typedef_node = llvm::dyn_cast<clang::TypedefNameDecl const>(&node))
        {
            return make_typedef(*typedef_node, name, file);
        }

        if (clang::TagDecl const* tag_node = llvm::dyn_cast<clang::TagDecl const>(&node))
        {
            // TODO: sanity checks
            if (tag_node->isStruct())
            {
                return make_struct(*llvm::dyn_cast<clang::RecordDecl const>(&node), name, file);
            }
            else if (tag_node->isEnum())
            {
                return make_enum(*llvm::dyn_cast<clang::EnumDecl const>(&node), name, file);
            }
            else if (tag_node->isUnion())
            {
                return make_union(*llvm::dyn_cast<clang::RecordDecl const>(&node), name, file);
            }
        }

        info() << "Unknown entity type '" << node.getDeclKindName() << "' encountered!" << std::endl;

        return code_entity();
    }

    // TODO: should log if same entry found multiple times and definition changes.
    code_entity code_entity_factory::make_enum(
        clang::EnumDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "code_entity_factory::make_enum('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        code_entity & entity = symbols.get(node.getQualifiedNameAsString());

        if (node.isCompleteDefinition() && !entity) // TODO: forward declarations for structs/unions?
        {
            enum_entity::values_map_type enum_values;

            for (auto const* enum_constant : node.enumerators())
            {
                std::string constant_name = enum_constant->getNameAsString();
                enum_values[constant_name] = enum_constant->getInitVal().getSExtValue();
            }

            entity = enum_entity(name, file, enum_values);
        }

        debug() << "code_entity_factory::make_enum() finished [entity = " << entity.get_impl() << "]" << std::endl;

        return entity;
    }

    code_entity code_entity_factory::make_function(
        clang::FunctionDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "code_entity_factory::make_function('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        code_entity & entity = symbols.get(node.getQualifiedNameAsString());

        code_entity const& return_type = get_dependent_type(node.getReturnType());

        function::arguments_list_type arguments;
        for (auto const* param : node.params())
        {
            code_entity const& param_type = get_dependent_type(param->getType());
            arguments.push_back(param_type);
        }

        entity = function(name, file, return_type, arguments);

        debug() << "code_entity_factory::make_function() finished [entity = " << entity.get_impl() << "]" << std::endl;

        return entity;
    }

    code_entity code_entity_factory::make_typedef(
        clang::TypedefNameDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "code_entity_factory::make_typedef('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        // don't create unnecessary typedefs
        if (fundamental_type_entity::is_supported(node.getUnderlyingType().getAsString()))
        {
            return get_dependent_type(node.getUnderlyingType());
        }

        code_entity & entity = symbols.get(node.getQualifiedNameAsString());

        code_entity const& alias_type = get_dependent_type(node.getUnderlyingType());
        entity = typedef_entity(name, file, alias_type);

        debug() << "code_entity_factory::make_typedef() finished [entity = " << entity.get_impl() << "]" << std::endl;

        return entity;
    }

    code_entity code_entity_factory::make_struct(
        clang::RecordDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "code_entity_factory::make_struct('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        code_entity & entity = symbols.get(node.isAnonymousStructOrUnion() ? "" : node.getQualifiedNameAsString());

        struct_entity::members_map_type members;
        for (auto const* field : node.fields())
        {
            std::string field_name = field->getNameAsString();
            members[field_name] = &get_dependent_type(field->getType());
        }

        entity = struct_entity(name, file, members, node.isAnonymousStructOrUnion());

        debug() << "code_entity_factory::make_struct() finished [entity = " << entity.get_impl() << "]" << std::endl;

        return entity;
    }

    code_entity code_entity_factory::make_union(
        clang::RecordDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "code_entity_factory::make_union('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        code_entity & entity = symbols.get(node.isAnonymousStructOrUnion() ? "" : node.getQualifiedNameAsString());

        union_entity::variants_map_type variants;
        for (auto const* field : node.fields())
        {
            std::string field_name = field->getNameAsString();
            variants[field_name] = &get_dependent_type(field->getType());
        }

        entity = union_entity(name, file, variants, node.isAnonymousStructOrUnion());

        debug() << "code_entity_factory::make_union() finished [entity = " << entity.get_impl() << "]" << std::endl;

        return entity;
    }

    code_entity const& code_entity_factory::get_dependent_type(clang::QualType type) const
    {
        code_entity & result = symbols.get(type.getAsString());
        if (result)
        {
            debug() << "'" << type.getAsString() << "' already in symbol table" << std::endl;

            return result;
        }

        clang::Type const* realType = type.getTypePtrOrNull();
        if (realType == nullptr) // sanity check
        {
            throw new std::runtime_error(
                std::string("QualType Type pointer is null, not sure how to handle this. type == ") +
                type.getAsString()
            );
        }

        // remove unneeded modifiers
        type.removeLocalConst();
        type.removeLocalVolatile();
        type.removeLocalRestrict();
        type.removeLocalFastQualifiers();

        debug() << "get_dependent_type(): inspecting type '" <<type.getAsString() << std::endl;

        if (realType->isArrayType())
        {
            clang::QualType element_type = static_cast<clang::ArrayType const*>(realType)->getElementType();

            debug() << "found array element type '" << element_type.getAsString() << "'" << std::endl;

            result = reference_entity(get_dependent_type(element_type));
        }
        else if (realType->isPointerType())
        {
            clang::QualType pointee_type = static_cast<clang::PointerType const*>(realType)->getPointeeType();

            // for some reason arrays like int abc[]; will be pointers on the outside w/ int[] the pointee type.
            // need to check if the pointee type is an array here to work around this.
            clang::Type const* pointee_type_real = pointee_type.getTypePtrOrNull();
            if (pointee_type_real && pointee_type_real->isArrayType()) {
                pointee_type = static_cast<clang::ArrayType const*>(pointee_type_real)->getElementType();
            }

            debug() << "found pointee type '" << pointee_type.getAsString() << "'" << std::endl;

            result = reference_entity(get_dependent_type(pointee_type));
        }
        else if (realType->isReferenceType())
        {
            clang::QualType pointee_type = static_cast<clang::ReferenceType const*>(realType)->getPointeeType();

            debug() << "found reference type '" << pointee_type.getAsString() << "'" << std::endl;

            result = reference_entity(get_dependent_type(pointee_type));
        }
        else if (fundamental_type_entity::is_supported(type.getAsString()))
        {
            result = fundamental_type_entity(type.getAsString());
        }
        else if (realType->isEnumeralType())
        {
            clang::EnumType const* enum_type = static_cast<clang::EnumType const*>(realType);

            clang::EnumDecl const* related_decl = enum_type->getDecl();
            if (related_decl)
            {
                clang::QualType enum_type;

                clang::TypeSourceInfo * tsi = related_decl->getIntegerTypeSourceInfo();
                if (tsi) {
                    enum_type = tsi->getType();
                } else {
                    enum_type = related_decl->getIntegerType();
                }

                if (!enum_type.isNull()) {
                    result = get_dependent_type(enum_type);
                } else {
                    warning() << "No enum type found for '" << type.getAsString() << "', defaulting to 'int'." << std::endl;

                    result = fundamental_type_entity("int");
                }
            }
        }

        if (!result)
        {
            debug() << "found type '" << type.getAsString() << "' not yet defined [canonical = '"
                    << realType->getCanonicalTypeInternal().getAsString() << "']" << std::endl;
        }

        // TODO: should handle 'char *' as string correctly? or will node-ffi know that ref.refType('char') can use string?

        return result;
    }
}

// TODO: should tie generation in w/ code_entities, instead of parallel generator hierarchy. maybe an inner class.
// TODO: 'load' is a special key