#include <llvm/Config/llvm-config.h>

#include <ffigen/process/parse_files.hpp>
#include <ffigen/process/symbol_table.hpp>
#include <ffigen/process/code_entity_factory.hpp>
#include <ffigen/utility/logger.hpp>
#include <ffigen/utility/exceptions.hpp>
#include <ffigen/utility/error_codes.hpp>

#include <llvm/Support/Host.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DataRecursiveASTVisitor.h>
#include <clang/Parse/Parser.h>
#include <clang/Parse/ParseAST.h>

#include <memory>
#include <iostream>

namespace ffigen
{
    using clang::CompilerInstance;
    using clang::TargetOptions;
    using clang::TargetInfo;
    using clang::FileEntry;
    using clang::Token;
    using clang::ASTContext;
    using clang::ASTConsumer;
    using clang::Parser;
    using clang::DiagnosticOptions;
    using clang::TextDiagnosticPrinter;

    using namespace utility::logs;

    // TODO: *_entity should be renamed _symbol
    struct TypeCapturer : public clang::DataRecursiveASTVisitor<TypeCapturer>
    {
        TypeCapturer(symbol_table & symbols, ASTContext * context)
            : symbols(symbols)
            , context(context)
            , factory(symbols, context->getSourceManager(), *context)
        {}

        bool VisitNamedDecl(clang::NamedDecl * node)
        {
            factory.make(*node);
            return true;
        }

        symbol_table & symbols;
        ASTContext * context;
        code_entity_factory factory;
    };

    struct TypeCapturerConsumer : public clang::ASTConsumer
    {
        TypeCapturerConsumer(symbol_table & symbols)
            : symbols(symbols)
        {}

        virtual void HandleTranslationUnit(clang::ASTContext & context)
        {
            TypeCapturer visitor(symbols, &context);
            visitor.TraverseDecl(context.getTranslationUnitDecl());
        }

        symbol_table & symbols;
    };

    void parse_files(std::list<std::string> const& files, symbol_table & symbols, std::list<std::string> const& include_directories)
    {
        std::shared_ptr<TargetOptions> target_options(new TargetOptions());
        target_options->Triple = llvm::sys::getDefaultTargetTriple();
        target_options->CPU = llvm::sys::getHostCPUName();

        info() << "  Clang targetinfo => [triple = '" << target_options->Triple << "', cpu = '" << target_options->CPU << "']" << std::endl;

        for (std::string const& file : files)
        {
            debug() << "parse_files(): starting parse for '" << file << "'" << std::endl;

            CompilerInstance ci; // TODO: if we don't create a new CompilerInstance for each header file, a segfault occurs.
                                 //       maybe I'm just not using clang correctly? happens when different files are used as the
                                 //       'main' file and include the same headers/types.
            DiagnosticOptions diagnosticOptions;

            ci.getPreprocessorOpts().UsePredefines = true;

            // TODO: should specify as argument
            for (auto const& path : include_directories)
            {
                ci.getHeaderSearchOpts().AddPath(llvm::StringRef(path.c_str()), clang::frontend::Angled, false, false);
                ci.getHeaderSearchOpts().AddPath(llvm::StringRef(path.c_str()), clang::frontend::Quoted, false, false);
            }

            ci.createDiagnostics();

            ci.setTarget(TargetInfo::CreateTargetInfo(ci.getDiagnostics(), target_options));

            ci.createFileManager();
            ci.createSourceManager(ci.getFileManager());
            ci.createPreprocessor(clang::TU_Complete);

            TypeCapturerConsumer * ast_consumer = new TypeCapturerConsumer(symbols);
            ci.setASTConsumer(std::unique_ptr<TypeCapturerConsumer>(ast_consumer));

            ci.createASTContext();

            std::cout << "Parsing file '" << file << "'..." << std::endl;

            FileEntry const* file_entry = ci.getFileManager().getFile(file);
            ci.getSourceManager().setMainFileID(
                ci.getSourceManager().createFileID(file_entry, clang::SourceLocation(), clang::SrcMgr::C_User));

            ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(), &ci.getPreprocessor());
            clang::ParseAST(
                ci.getPreprocessor(),
                ast_consumer,
                ci.getASTContext(),
                false,                     // print stats
                clang::TU_Complete,
                nullptr,                   // code completion
                true                       // skip function bodies
            );
            ci.getDiagnosticClient().EndSourceFile();

            if (ci.getDiagnostics().hasErrorOccurred()) {
                throw fatal_error(std::string("Errors in '") + file + "', aborting generation.", error_codes::HEADER_PARSE_FAIL);
            }

            debug() << "parse_files(): finished parse for '" << file << "'" << std::endl;
        }

        debug() << "parse_files(): finished parsing all files" << std::endl;
    }
}