cmake_minimum_required(VERSION 2.8.8)
project(node-ffi-generator)

link_directories(/home/runic/Tools/llvm/Release/lib)
include_directories(
	/home/runic/Tools/llvm/Release/include
	/home/runic/Tools/llvm/Release/tools/clang/include
	libs/argumentative/include
	libs/filesystem/include
	libs/boostalgorithm/include
	libs/boostsystem/include
	libs/boostpredef/include
	include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fPIC -fvisibility-inlines-hidden -Wall -W -Wno-unused-parameter -Wwrite-strings -Wno-strict-aliasing -Wno-missing-field-initializers -pedantic -Wno-long-long -Wno-comment -std=c++11 -ffunction-sections -fdata-sections   -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -DBOOST_NO_TYPEID -DBOOST_NO_RTTI")

set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -flavor gnu")

# Now build our tools
add_executable(node-ffi-generator
    # boost/filesystem source
	libs/filesystem/src/codecvt_error_category.cpp
	libs/filesystem/src/operations.cpp
	libs/filesystem/src/path_traits.cpp
	libs/filesystem/src/path.cpp
	libs/filesystem/src/portability.cpp
	libs/filesystem/src/unique_path.cpp
	libs/filesystem/src/utf8_codecvt_facet.cpp

	# boost/system source
	libs/boostsystem/src/error_code.cpp

	# node-ffi-generator source
	source/main.cpp
	source/generate_node_ffi_interface.cpp
	source/process/code_entity_factory.cpp
	source/process/parse_files.cpp
	source/process/symbol_table.cpp
	source/process/code_entity.cpp
	source/process/code_entity/fundamental_type.cpp
	source/process/code_entity/lazy_code_entity.cpp
	source/process/code_entity/function_pointer_entity.cpp
	source/process/code_entity/reference.cpp
	source/generate/generate.cpp
	source/generate/generate_js_files.cpp
	source/generate/generator_factory.cpp
	source/generate/generator/enum_map_generator.cpp
	source/generate/generator/function_map_generator.cpp
	source/generate/generator/library_load_generator.cpp
	source/generate/generator/typedef_map_generator.cpp
	source/generate/generator/record_map_generator.cpp
	source/utility/logger.cpp
)

# Link against LLVM libraries (assumes lld, not system ld so libs not in order)
target_link_libraries(node-ffi-generator
	clangFrontendTool
	clangFrontend
	clangDriver
	clangSerialization
	clangCodeGen
	clangParse
	clangSema
	clangStaticAnalyzerFrontend
	clangStaticAnalyzerCheckers
	clangStaticAnalyzerCore
	clangAnalysis
	clangARCMigrate
	clangRewriteFrontend
	clangRewrite
	clangEdit
	clangAST
	clangLex
	clangBasic

    LLVMCppBackendCodeGen
    LLVMTarget
    LLVMMC
    LLVMCore
    LLVMCppBackendInfo
    LLVMSupport
    LLVMOption
    LLVMBitReader
    LLVMBitWriter
    LLVMMCParser

	rt
	dl
	tinfo
	atomic
	pthread
	z
)

# TODO: use cakejam.
