set(LOGGER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/loggers.c" PARENT_SCOPE)

add_library(Logger SHARED pass.cpp)
target_include_directories(Logger PRIVATE ${LLVM_INCLUDE_DIRS})