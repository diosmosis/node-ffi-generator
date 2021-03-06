#include <llvm/Config/llvm-config.h>

#include <ffigen/process/symbol_table.hpp>
#include <ffigen/process/symbol_factory.hpp>
#include <ffigen/process/clang_facade.hpp>
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
#include <clang/Lex/HeaderSearch.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DataRecursiveASTVisitor.h>
#include <clang/Parse/Parser.h>
#include <clang/Parse/ParseAST.h>

#include <unordered_set>
#include <memory>
#include <iostream>
#include <sstream>

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

    struct TypeCapturer : public clang::DataRecursiveASTVisitor<TypeCapturer>
    {
        TypeCapturer(symbol_table & symbols, ASTContext * context,
                     std::list<clang_facade::symbol_filter_type> const& filters)
            : symbols(symbols)
            , context(context)
            , symbol_filters(filters)
            , factory(symbols, context->getSourceManager(), *context)
        {}

        bool VisitNamedDecl(clang::NamedDecl * node)
        {
            for (auto const& filter : symbol_filters) {
                if (!filter(*node)) {
                    return true;
                }
            }

            factory.make(*node);
            return true;
        }

        symbol_table & symbols;
        ASTContext * context;
        std::list<clang_facade::symbol_filter_type> const& symbol_filters;
        symbol_factory factory;
    };

    struct TypeCapturerConsumer : public clang::ASTConsumer
    {
        TypeCapturerConsumer(symbol_table & symbols, std::list<clang_facade::symbol_filter_type> const& filters)
            : symbols(symbols)
            , symbol_filters(filters)
        {}

        virtual void HandleTranslationUnit(clang::ASTContext & context)
        {
            TypeCapturer visitor(symbols, &context, symbol_filters);
            visitor.TraverseDecl(context.getTranslationUnitDecl());
        }

        symbol_table & symbols;
        std::list<clang_facade::symbol_filter_type> const& symbol_filters;
    };

    struct AssumePragmaOnce : public clang::PPCallbacks
    {
        AssumePragmaOnce(clang::HeaderSearch & header_search)
            : header_search(header_search)
        {}

        void InclusionDirective(
            clang::SourceLocation HashLoc,
            clang::Token const& IncludeTok,
            llvm::StringRef FileName,
            bool IsAngled,
            clang::CharSourceRange FilenameRange,
            clang::FileEntry const* File,
            llvm::StringRef SearchPath,
            llvm::StringRef RelativePath,
            clang::Module const* Imported
        ) {
            if (included.find(File) != included.end()) {
                header_search.MarkFileIncludeOnce(File);
            } else {
                included.insert(File);
            }
        }

        clang::HeaderSearch & header_search;
        std::unordered_set<clang::FileEntry const*> included;
    };

    void clang_facade::parse_files(symbol_table & symbols) const
    {
        std::shared_ptr<TargetOptions> target_options(new TargetOptions());
        target_options->Triple = llvm::sys::getDefaultTargetTriple();
        target_options->CPU = llvm::sys::getHostCPUName();

        info() << "  Clang targetinfo => [triple = '" << target_options->Triple << "', cpu = '" << target_options->CPU << "']" << std::endl;

        for (std::string const& file : files_to_process) {
            debug() << "parse_files(): starting parse for '" << file << "'" << std::endl;

            CompilerInstance ci; // TODO: if we don't create a new CompilerInstance for each header file, a segfault occurs.
                                 //       maybe I'm just not using clang correctly? happens when different files are used as the
                                 //       'main' file and include the same headers/types.
            DiagnosticOptions diagnosticOptions;

            ci.getPreprocessorOpts().UsePredefines = true;

            // TODO: should specify as argument
            // add include directories
            for (auto const& path : include_directories) {
                ci.getHeaderSearchOpts().AddPath(llvm::StringRef(path.c_str()), clang::frontend::Angled, false, false);
                ci.getHeaderSearchOpts().AddPath(llvm::StringRef(path.c_str()), clang::frontend::Quoted, false, false);
            }

            ci.createDiagnostics();

            ci.setTarget(TargetInfo::CreateTargetInfo(ci.getDiagnostics(), target_options));

            ci.createFileManager();
            ci.createSourceManager(ci.getFileManager());
            ci.createPreprocessor(clang::TU_Complete);

            // add predefines to preprocessor
            {
                std::stringstream ss;
                ss << "//"; // first line in predefines appears to be invalid...
                ss << ci.getPreprocessor().getPredefines() << "\n";

                for (auto const& pair : predefines) { // TODO: should probably use clang's MacroBuilder
                    ss << "#define " << pair.first << ' ' << pair.second << '\n';
                }

                ci.getPreprocessor().setPredefines(ss.str());
            }

            // add callbacks to preprocessor
            if (assume_pragma_once) {
                ci.getPreprocessor().addPPCallbacks(std::unique_ptr<clang::PPCallbacks>(
                    new AssumePragmaOnce(ci.getPreprocessor().getHeaderSearchInfo())
                ));
            }

            TypeCapturerConsumer * ast_consumer = new TypeCapturerConsumer(symbols, symbol_filters);
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

        symbols.resolve_lazy_symbols();

        debug() << "parse_files(): resolved lazy symbols in symbol table" << std::endl;
    }
}