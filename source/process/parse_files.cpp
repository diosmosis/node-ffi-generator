#include <llvm/Config/llvm-config.h>

#include <ffigen/process/parse_files.hpp>
#include <ffigen/process/symbol_table.hpp>
#include <ffigen/process/code_entity/function.hpp>
#include <ffigen/process/code_entity/struct.hpp>
#include <ffigen/process/code_entity/typedef.hpp>
#include <ffigen/process/code_entity/union.hpp>
#include <ffigen/process/code_entity_factory.hpp>
#include <ffigen/utility/logger.hpp>

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
            , factory(symbols, context->getSourceManager())
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

	// TODO: will need lots of debug logging.

	void parse_files(std::list<std::string> const& files, symbol_table & symbols)
	{
	    CompilerInstance ci;
    	DiagnosticOptions diagnosticOptions;

        ci.getPreprocessorOpts().UsePredefines = false;

        // TODO: should specify as argument
        ci.getHeaderSearchOpts().AddPath(llvm::StringRef("/usr/include/"), clang::frontend::Angled, false, false);

	    ci.createDiagnostics();

	    std::shared_ptr<TargetOptions> target_options(new TargetOptions());
	    target_options->Triple = llvm::sys::getDefaultTargetTriple();
    	ci.setTarget(TargetInfo::CreateTargetInfo(ci.getDiagnostics(), target_options));

	    ci.createFileManager();
	    ci.createSourceManager(ci.getFileManager());
	    ci.createPreprocessor(clang::TU_Complete);

	    TypeCapturerConsumer * ast_consumer = new TypeCapturerConsumer(symbols);
	    ci.setASTConsumer(std::unique_ptr<TypeCapturerConsumer>(ast_consumer));

	    ci.createASTContext();

	    for (std::string const& file : files)
	    {
	    	FileEntry const* file_entry = ci.getFileManager().getFile(file);
	    	ci.getSourceManager().setMainFileID( // TODO: can't add multiple files?
	    		ci.getSourceManager().createFileID(file_entry, clang::SourceLocation(), clang::SrcMgr::C_User));

	    	ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(), &ci.getPreprocessor());
	    	clang::ParseAST(ci.getPreprocessor(), ast_consumer, ci.getASTContext());
	    	ci.getDiagnosticClient().EndSourceFile();
	    }
	}
}