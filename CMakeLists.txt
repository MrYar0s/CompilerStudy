cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(compiler_project)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set(PROJECT_BINARY_ROOT ${PROJECT_BINARY_DIR})
set(PROJECT_SCRIPTS ${PROJECT_ROOT}/scripts)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_RELEASE "-O2")

find_package(Python3 REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window)
set(SFML_LIBS
	sfml-graphics
	sfml-window
	sfml-system
	sfml-network
	sfml-audio
)

add_custom_target(
		run-clang-format
		COMMAND ${Python3_EXECUTABLE} ${PROJECT_SCRIPTS}/run_clang_format.py ${PROJECT_ROOT}
)

add_subdirectory(llvm-pass)
add_subdirectory(src)