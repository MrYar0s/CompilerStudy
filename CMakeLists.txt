cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(compiler_project)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set(PROJECT_BINARY_ROOT ${PROJECT_BINARY_DIR})
set(PROJECT_SCRIPTS ${PROJECT_ROOT}/scripts)

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

add_subdirectory(src)