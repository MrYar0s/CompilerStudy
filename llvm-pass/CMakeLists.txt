find_package(LLVM REQUIRED)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

add_library(Logger SHARED pass.cpp)
target_include_directories(Logger PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_options(Logger PUBLIC -flegacy-pass-manager)