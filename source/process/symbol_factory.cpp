#include <llvm/Config/llvm-config.h>
#include <ffigen/process/symbol_factory.hpp>
#include <ffigen/process/symbol/enum_symbol.hpp>
#include <ffigen/process/symbol/function_symbol.hpp>
#include <ffigen/process/symbol/record_symbol.hpp>
#include <ffigen/process/symbol/typedef_symbol.hpp>
#include <ffigen/process/symbol/reference_symbol.hpp>
#include <ffigen/process/symbol/fundamental_type_symbol.hpp>
#include <ffigen/process/symbol/array_symbol.hpp>
#include <ffigen/process/symbol/function_pointer_symbol.hpp>
#include <ffigen/utility/logger.hpp>
#include <ffigen/utility/exceptions.hpp>
#include <ffigen/utility/error_codes.hpp>
#include <clang/AST/ASTContext.h>
#include <stdexcept>
#include <iostream>

namespace ffigen
{
    using namespace utility::logs;

    namespace detail
    {
        static std::string clean_type(std::string const& name)
        {
            if (name.compare(0, 7, "struct ") == 0) {
                return name.substr(7);
            } else if (name.compare(0, 6, "union ") == 0) {
                return name.substr(6);
            } else {
                return name;
            }
        }

        static bool is_anonymous_type(std::string const& name)
        {
            return name.find("(anonymous") != std::string::npos;
        }

        static bool is_anonymous_record(clang::RecordDecl const& node)
        {
            return node.isAnonymousStructOrUnion()
                || is_anonymous_type(node.getQualifiedNameAsString());
        }
    }

    symbol symbol_factory::make(clang::NamedDecl const& node) const
    {
        debug() << "symbol_factory::make('" << node.getNameAsString() << "')" << std::endl;

        clang::PresumedLoc loc = source_manager.getPresumedLoc(node.getLocation());

        std::string name = node.getNameAsString();
        std::string file = loc.getFilename();

        debug() << "symbol_factory::make(): [ name = '" << name << "', file = '" << file << "' ]" << std::endl;

        if (clang::EnumDecl const* enum_node = llvm::dyn_cast<clang::EnumDecl const>(&node)) {
            return make_enum(*enum_node, name, file);
        }

        if (clang::FunctionDecl const* function_node = llvm::dyn_cast<clang::FunctionDecl const>(&node)) {
            return make_function(*function_node, name, file);
        }

        if (clang::TypedefNameDecl const* typedef_node = llvm::dyn_cast<clang::TypedefNameDecl const>(&node)) {
            return make_typedef(*typedef_node, name, file);
        }

        if (clang::TagDecl const* tag_node = llvm::dyn_cast<clang::TagDecl const>(&node)) {
            if (tag_node->isStruct()
                || tag_node->isUnion()
            ) {
                return make_record(*llvm::dyn_cast<clang::RecordDecl const>(&node), name, file);
            } else if (tag_node->isEnum()) {
                return make_enum(*llvm::dyn_cast<clang::EnumDecl const>(&node), name, file);
            }
        }

        debug() << "symbol_factory::make(): Unknown node type '" << node.getDeclKindName()
                << "' encountered!" << std::endl;

        std::string accessed_name = node.getNameAsString() + " [type = " + node.getDeclKindName() + "]";
        return symbol(accessed_name);
    }

    // TODO: should log if same entry found multiple times and definition changes.
    symbol symbol_factory::make_enum(
        clang::EnumDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "symbol_factory::make_enum('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        symbol & entity = symbols.get(node.getQualifiedNameAsString());

        if (entity
            && entity.is_a<enum_symbol>()
        ) {
            debug() << "symbol_factory::make_enum(): enum '" << name << "' already exists" << std::endl;

            return entity;
        }

        if (node.isCompleteDefinition() && !entity) {
            enum_symbol::values_map_type enum_values;

            for (auto const* enum_constant : node.enumerators()) {
                std::string constant_name = enum_constant->getNameAsString();
                enum_values[constant_name] = enum_constant->getInitVal().getSExtValue();
            }

            entity = enum_symbol(name, file, enum_values);
        }

        debug() << "symbol_factory::make_enum() finished [entity = " << entity.get_impl() << "]" << std::endl;

        return entity;
    }

    symbol symbol_factory::make_function(
        clang::FunctionDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "symbol_factory::make_function('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        std::string function_id = node.getQualifiedNameAsString() + "()";

        symbol & entity = symbols.get(function_id);

        if (entity
            && entity.is_a<function_symbol>()
        ) {
            debug() << "symbol_factory::make_function(): function '" << name << "' already exists" << std::endl;

            return entity;
        }

        symbol const& return_type = get_dependent_type(node.getReturnType());

        function_symbol::arguments_list_type arguments;
        for (auto const* param : node.params()) {
            symbol const& param_type = get_dependent_type(param->getType());
            arguments.push_back(param_type);
        }

        entity = function_symbol(name, file, return_type, arguments, node.isVariadic());

        debug() << "symbol_factory::make_function() finished [entity = " << entity.get_impl() << "]" << std::endl;

        return entity;
    }

    symbol symbol_factory::make_typedef(
        clang::TypedefNameDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "symbol_factory::make_typedef('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        symbol & entity = symbols.get(node.getQualifiedNameAsString());

        if (entity
            && entity.is_a<typedef_symbol>()
        ) {
            debug() << "symbol_factory::make_typedef(): typedef '" << name << "' already exists" << std::endl;

            return entity;
        }

        symbol & alias_type = get_dependent_type(node.getUnderlyingType());

        // don't create unnecessary typedefs
        if (alias_type.is_a<record_symbol>()
            && alias_type.is_anonymous()
        ) {
            alias_type.cast<record_symbol>().deanonymize(name);

            entity = alias_type;
        } else if (fundamental_type_symbol::is_supported(node.getUnderlyingType().getAsString())) {
            debug() << "symbol_factory::make_typedef(): typedef of fundamental type found" << std::endl;

            entity = alias_type;
        } else {
            entity = typedef_symbol(name, file, alias_type);

            debug() << "symbol_factory::make_typedef() finished [entity = " << entity.get_impl() << "]" << std::endl;
        }

        return entity;
    }

    symbol symbol_factory::make_record(
        clang::RecordDecl const& node, std::string const& name, std::string const& file) const
    {
        debug() << "symbol_factory::make_record('" << node.getQualifiedNameAsString() << "', '"
                << name << "', '" << file << "')" << std::endl;

        bool is_anonymous = detail::is_anonymous_record(node);

        debug() << "symbol_factory::make_record(): is anonymous == " << is_anonymous << std::endl;

        std::string symbol_name = node.getQualifiedNameAsString();
        if (is_anonymous) {
            symbol_name = detail::clean_type(node.getTypeForDecl()->getCanonicalTypeInternal().getAsString());
        }

        symbol & entity = symbols.get(symbol_name);

        if (!node.isCompleteDefinition()) {
            return entity;
        }

        if (entity
            && entity.is_a<record_symbol>()
        ) {
            debug() << "symbol_factory::make_record(): record_symbol '" << name << "' already exists" << std::endl;

            return entity;
        }

        record_symbol::members_map_type members;
        for (auto const* field : node.fields()) {
            std::string field_name = field->getNameAsString();
            members.push_back(std::make_pair(field_name, get_dependent_type(field->getType())));
        }

        entity = record_symbol(name.empty() ? symbol_name : name, file, members, is_anonymous, node.isUnion());

        debug() << "symbol_factory::make_record() finished [entity = " << entity.get_impl() << "]" << std::endl;

        return entity;
    }

    symbol & symbol_factory::get_dependent_type(clang::QualType type) const
    {
        // remove unneeded modifiers
        type.removeLocalConst();
        type.removeLocalVolatile();
        type.removeLocalRestrict();
        type.removeLocalFastQualifiers();

        clang::Type const* realType = type.getTypePtrOrNull();
        if (realType == nullptr) { // sanity check 
            throw std::runtime_error(
                std::string("QualType Type pointer is null, not sure how to handle this. type == ") +
                type.getAsString()
            );
        }

        std::string clean_type_string;
        if (detail::is_anonymous_type(type.getAsString())) {
            clean_type_string = detail::clean_type(realType->getCanonicalTypeInternal().getAsString());
        } else {
            clean_type_string = detail::clean_type(type.getAsString());
        }

        symbol & result = symbols.get(clean_type_string);
        if (result) {
            debug() << "'" << clean_type_string << "' already in symbol table" << std::endl;

            return result;
        }

        debug() << "get_dependent_type(): inspecting type '" << clean_type_string << "' [real ptr = " << realType << "]" << std::endl;

        if (fundamental_type_symbol::is_supported(clean_type_string)) {
            debug() << "found fundamental type '" << clean_type_string << "'" << std::endl;

            result = fundamental_type_symbol(clean_type_string);
        } else if (realType->isConstantArrayType()) {
            clang::ConstantArrayType const* arrayType = static_cast<clang::ConstantArrayType const*>(realType);
            clang::QualType element_type = arrayType->getElementType();

            debug() << "found fixed size array element type '" << element_type.getAsString() << "'" << std::endl;

            result = array_symbol(get_dependent_type(element_type), arrayType->getSize().getSExtValue());
        } else if (realType->isArrayType()) {
            clang::QualType element_type = static_cast<clang::ArrayType const*>(realType)->getElementType();

            debug() << "found array element type '" << element_type.getAsString() << "'" << std::endl;

            result = reference_symbol(get_dependent_type(element_type));
        } else if (realType->isFunctionType()) {
            debug() << "found function type '" << clean_type_string << "'" << std::endl;

            clang::Type const* function_type_impl = realType->getCanonicalTypeInternal().getTypePtr();
            if (!function_type_impl->isFunctionProtoType()) {
                throw fatal_error("No-proto function type found '" + clean_type_string + "'! Can't get argument info.",
                    error_codes::UNSUPPORTED);
            }

            clang::FunctionProtoType const* functionType = static_cast<clang::FunctionProtoType const*>(function_type_impl);

            symbol return_type = get_dependent_type(functionType->getReturnType());

            function_pointer_symbol::arguments_list_type param_types;
            for (auto const& param_type : functionType->param_types()) {
                param_types.push_back(get_dependent_type(param_type));
            }

            result = function_pointer_symbol(clean_type_string, return_type, param_types);
        } else if (realType->isPointerType()) {
            clang::QualType pointee_type = static_cast<clang::PointerType const*>(realType)->getPointeeType();

            // for some reason arrays like int abc[]; will be pointers on the outside w/ int[] the pointee type.
            // need to check if the pointee type is an array here to work around this.
            clang::Type const* pointee_type_real = pointee_type.getTypePtrOrNull();
            if (pointee_type_real && pointee_type_real->isArrayType()) {
                pointee_type = static_cast<clang::ArrayType const*>(pointee_type_real)->getElementType();
            }

            debug() << "found pointee type '" << pointee_type.getAsString() << "'" << std::endl;

            result = reference_symbol(get_dependent_type(pointee_type));
        } else if (realType->isReferenceType()) {
            clang::QualType pointee_type = static_cast<clang::ReferenceType const*>(realType)->getPointeeType();

            debug() << "found reference type '" << pointee_type.getAsString() << "'" << std::endl;

            result = reference_symbol(get_dependent_type(pointee_type));
        } else if (realType->isEnumeralType()) {
            int64_t size_of_enum = context.getTypeSizeInChars(realType).getQuantity();

            result = fundamental_type_symbol::make_int_from_size(size_of_enum);
        }

        if (!result) {
            debug() << "found type '" << clean_type_string << "' not yet defined [canonical = '"
                    << realType->getCanonicalTypeInternal().getAsString() << "']" << std::endl;
        }

        return result;
    }
}