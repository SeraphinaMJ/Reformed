cmake_minimum_required(VERSION 3.15.0)

project(TempestPrebuilder)

# download and unpack clang at configuration time
configure_file(Clang.in ${CMAKE_BINARY_DIR}/clang-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/clang-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/clang-download" )

# setup the path to python (needed for school computers due to cygwin being higher in path)
set(PYTHON_EXECUTABLE "C:/Python27/python.exe" CACHE FILEPATH "")

# setup clang options
set(LLVM_ENABLE_PROJECTS "clang" CACHE STRING "")
set(LLVM_TARGETS_TO_BUILD "X86" CACHE STRING "")
set(LLVM_INCLUDE_BENCHMARKS OFF CACHE BOOL "")
set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "")
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "")
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "")
set(LLVM_INCLUDE_GO_TESTS OFF CACHE BOOL "")
set(LLVM_INCLUDE_TOOLS ON CACHE BOOL "")
set(LLVM_INCLUDE_UTILS ON CACHE BOOL "")
set(CLANG_BUILD_TOOLS OFF CACHE BOOL "")

# add llvm/clang directory to our build
add_subdirectory("${CMAKE_BINARY_DIR}/clang-src/llvm" "${CMAKE_BINARY_DIR}/clang-build")

# create our prebuilder target
add_executable(TempestPrebuilder
	stdafx.cpp stdafx.h
	main.cpp
	TempestClassParser.h TempestClassParser.cpp
	TempestPrebuildSetup.h TempestPrebuildSetup.cpp
	TempestSerializationGenerator.h TempestSerializationGenerator.cpp
	ClangToolFrontEnd.h
	)
	
# bring in LLVM so we can link to it
find_package(LLVM REQUIRED CONFIG PATHS "${CMAKE_BINARY_DIR}/clang-build/lib/cmake/llvm" NO_DEFAULT_PATH)

# attach to LLVM targets
target_include_directories(TempestPrebuilder
	PRIVATE ${LLVM_INCLUDE_DIRS}
	PRIVATE "${CMAKE_BINARY_DIR}/clang-build/tools/clang/include"
	PRIVATE "${CMAKE_BINARY_DIR}/clang-src/clang/include"
	)
	
target_link_libraries(TempestPrebuilder
	PRIVATE clangAST
	PRIVATE clangFrontend
	PRIVATE clangTooling
	PRIVATE clangFrontendTool
	PRIVATE libclang
	)
	
target_compile_definitions(TempestPrebuilder
	PUBLIC "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
	)
	
# set C++ standard
set_property(TARGET TempestPrebuilder 
	PROPERTY CXX_STANDARD 17)
	
# Organize the targets into folders
set_target_properties(
	arcmt-test
	c-arcmt-test
	c-index-test
	clang
	clang-check
	clang-diff
	clang-extdef-mapping
	clang-format
	clang-fuzzer
	clang-import-test
	clang-offload-bundler
	clang-refactor
	clang-rename
	clang-scan-deps
	diagtool
	PROPERTIES FOLDER "ThirdParty/Clang/Clang executables")
set_target_properties(
	clangAnalysis
	clangARCMigrate
	clangAST
	clangASTMatchers
	clangBasic
	clangCodeGen
	clangCrossTU
	clangDependencyScanning
	clangDirectoryWatcher
	clangDriver
	clangDynamicASTMatchers
	clangEdit
	clangFormat
	clangFrontend
	clangFrontendTool
	clangHandleCXX
	clangHandleLLVM
	clangIndex
	clangLex
	clangParse
	clangRewrite
	clangRewriteFrontend
	clangSema
	clangSerialization
	clangStaticAnalyzerCheckers
	clangStaticAnalyzerCore
	clangStaticAnalyzerFrontend
	clangTooling
	clangToolingASTDiff
	clangToolingCore
	clangToolingInclusions
	clangToolingRefactoring
	clangToolingSyntax
	libclang
	PROPERTIES FOLDER "ThirdParty/Clang/Clang libraries"
)
set_target_properties(
	ClangARMFP16
	ClangARMNeon
	ClangAttrClasses
	ClangAttrHasAttributeImpl
	ClangAttrImpl
	ClangAttrList
	ClangAttrParsedAttrImpl
	ClangAttrParsedAttrKinds
	ClangAttrParsedAttrList
	ClangAttrParserStringSwitches
	ClangAttrPCHRead
	ClangAttrPCHWrite
	ClangAttrSpellingListIndex
	ClangAttrSubjectMatchRuleList
	ClangAttrSubMatchRulesParserStringSwitches
	ClangAttrTemplateInstantiate
	ClangAttrTextDump
	ClangAttrTraverse
	ClangAttrVisitor
	ClangCommentCommandInfo
	ClangCommentCommandList
	ClangCommentHTMLNamedCharacterReferences
	ClangCommentHTMLTags
	ClangCommentHTMLTagsProperties
	ClangCommentNodes
	ClangDeclNodes
	ClangDiagnosticAnalysis
	ClangDiagnosticAST
	ClangDiagnosticComment
	ClangDiagnosticCommon
	ClangDiagnosticCrossTU
	ClangDiagnosticDriver
	ClangDiagnosticFrontend
	ClangDiagnosticGroups
	ClangDiagnosticIndexName
	ClangDiagnosticLex
	ClangDiagnosticParse
	ClangDiagnosticRefactoring
	ClangDiagnosticSema
	ClangDiagnosticSerialization
	ClangOpenCLBuiltinsImpl
	ClangSACheckers
	ClangStmtNodes
	clang-tblgen
	StmtDataCollectors
	PROPERTIES FOLDER "ThirdParty/Clang/Clang tablegenning"
)
set_target_properties(
	llvm-isel-fuzzer
	llvm-itanium-demangle-fuzzer
	llvm-microsoft-demangle-fuzzer
	llvm-opt-fuzzer
	llvm-special-case-list-fuzzer
	llvm-yaml-numeric-parser-fuzzer
	PROPERTIES FOLDER "ThirdParty/Clang/Fuzzers"
)
set_target_properties(
	LLVMAggressiveInstCombine
	LLVMAnalysis
	LLVMAsmParser
	LLVMAsmPrinter
	LLVMBinaryFormat
	LLVMBitReader
	LLVMBitstreamReader
	LLVMBitWriter
	LLVMCFIVerify
	LLVMCodeGen
	LLVMCore
	LLVMCoroutines
	LLVMCoverage
	LLVMDebugInfoCodeView
	LLVMDebugInfoDWARF
	LLVMDebugInfoGSYM
	LLVMDebugInfoMSF
	LLVMDebugInfoPDB
	LLVMDemangle
	LLVMDlltoolDriver
	LLVMExecutionEngine
	LLVMExegesis
	LLVMExegesisX86
	LLVMFuzzMutate
	LLVMGlobalISel
	LLVMInstCombine
	LLVMInstrumentation
	LLVMInterpreter
	LLVMipo
	LLVMIRReader
	LLVMJITLink
	LLVMLibDriver
	LLVMLineEditor
	LLVMLinker
	LLVMLTO
	LLVMMC
	LLVMMCA
	LLVMMCDisassembler
	LLVMMCJIT
	LLVMMCParser
	LLVMMIRParser
	LLVMObjCARCOpts
	LLVMObject
	LLVMObjectYAML
	LLVMOption
	LLVMOrcJIT
	LLVMPasses
	LLVMProfileData
	LLVMRemarks
	LLVMRuntimeDyld
	LLVMScalarOpts
	LLVMSelectionDAG
	LLVMSupport
	LLVMSymbolize
	LLVMTableGen
	LLVMTarget
	LLVMTextAPI
	LLVMTransformUtils
	LLVMVectorize
	LLVMWindowsManifest
	LLVMX86AsmParser
	LLVMX86CodeGen
	LLVMX86Desc
	LLVMX86Disassembler
	LLVMX86Info
	LLVMX86Utils
	LLVMXRay
	LTO
	Remarks
	PROPERTIES FOLDER "ThirdParty/Clang/Libraries"
)
set_target_properties(
	BugpointPasses
	LLVMHello
	PROPERTIES FOLDER "ThirdParty/Clang/Loadable modules"
)
set_target_properties(
	clang-headers
	clang-libraries
	clang-resource-headers
	clang-tablegen-targets
	libclang-headers
	llvm_vcsrevision_h
	llvm-headers
	llvm-libraries
	LTO_exports
	ocaml_all
	ocaml_make_directory
	Remarks_exports
	scan-build
	scan-view
	srpm
	PROPERTIES FOLDER "ThirdParty/Clang/Misc"
)
set_target_properties(
	obj.clangAnalysis
	obj.clangARCMigrate
	obj.clangAST
	obj.clangASTMatchers
	obj.clangBasic
	obj.clangCodeGen
	obj.clangCrossTU
	obj.clangDependencyScanning
	obj.clangDirectoryWatcher
	obj.clangDriver
	obj.clangDynamicASTMatchers
	obj.clangEdit
	obj.clangFormat
	obj.clangFrontend
	obj.clangFrontendTool
	obj.clangHandleCXX
	obj.clangHandleLLVM
	obj.clangIndex
	obj.clangLex
	obj.clangParse
	obj.clangRewrite
	obj.clangRewriteFrontend
	obj.clangSema
	obj.clangSerialization
	obj.clangStaticAnalyzerCheckers
	obj.clangStaticAnalyzerCore
	obj.clangStaticAnalyzerFrontend
	obj.clangTooling
	obj.clangToolingASTDiff
	obj.clangToolingCore
	obj.clangToolingInclusions
	obj.clangToolingRefactoring
	obj.clangToolingSyntax
	obj.clang-tblgen
	obj.llvm-tblgen
	PROPERTIES FOLDER "ThirdParty/Clang/Object Libraries"
)
set_target_properties(
	AttributeCompatFuncTableGen
	ClangDriverOptions
	CvtResTableGen
	DllOptionsTableGen
	InstCombineTableGen
	intrinsics_gen
	LibOptionsTableGen
	LipoOptsTableGen
	llvm-tblgen
	MtTableGen
	ObjcopyOptsTableGen
	RcTableGen
	StripOptsTableGen
	X86CommonTableGen
	PROPERTIES FOLDER "ThirdParty/Clang/Tablegenning"
)
set_target_properties(
	bugpoint
	dsymutil
	llc
	lli
	llvm-addr2line
	llvm-ar
	llvm-as
	llvm-bcanalyzer
	llvm-cat
	llvm-cfi-verify
	llvm-config
	llvm-cov
	llvm-c-test
	llvm-cvtres
	llvm-cxxdump
	llvm-cxxfilt
	llvm-cxxmap
	llvm-diff
	llvm-dis
	llvm-dlltool
	llvm-dwarfdump
	llvm-dwp
	llvm-elfabi
	llvm-exegesis
	llvm-extract
	llvm-jitlink
	llvm-lib
	llvm-link
	llvm-lipo
	llvm-lto
	llvm-lto2
	llvm-mc
	llvm-mca
	llvm-modextract
	llvm-mt
	llvm-nm
	llvm-objcopy
	llvm-objdump
	llvm-opt-report
	llvm-pdbutil
	llvm-profdata
	llvm-ranlib
	llvm-rc
	llvm-readelf
	llvm-readobj
	llvm-rtdyld
	llvm-size
	llvm-split
	llvm-stress
	llvm-strings
	llvm-strip
	llvm-symbolizer
	llvm-undname
	llvm-xray
	obj2yaml
	opt
	sancov
	sanstats
	verify-uselistorder
	yaml2obj
	PROPERTIES FOLDER "ThirdParty/Clang/Tools"
)
set_target_properties(
	ClangVisualizers
	count
	FileCheck
	hmaptool
	lli-child-target
	llvm-PerfectShuffle
	LLVMVisualizers
	not
	yaml-bench
	PROPERTIES FOLDER "ThirdParty/Clang/Utils"
)
set_target_properties(
	distribution
	install-distribution
	install-distribution-stripped
	PROPERTIES FOLDER "ThirdParty/Clang"
)