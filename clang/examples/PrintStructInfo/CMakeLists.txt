# File: llvm-project/clang/examples/PrintStructInfo/CMakeLists.txt

add_llvm_library(PrintStructInfo MODULE PrintStructInfo.cpp PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(PrintStructInfo PRIVATE
    clangAST
    clangBasic
    clangFrontend
    )
endif()