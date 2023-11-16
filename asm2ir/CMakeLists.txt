add_executable(asm2ir main.cpp)

target_include_directories(asm2ir PRIVATE ${LLVM_INCLUDE_DIRS})

target_link_libraries(asm2ir PRIVATE LLVM sfml_api)

target_include_directories(asm2ir PRIVATE ${PROJECT_ROOT}/src/api)

add_custom_target(run_asm
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/asm2ir ${CMAKE_CURRENT_SOURCE_DIR}/main.asm 1
	DEPENDS asm2ir
)

add_custom_target(dump_asm2ir
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/asm2ir ${CMAKE_CURRENT_SOURCE_DIR}/main.asm
	DEPENDS asm2ir
)

add_custom_target(run_ir
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/asm2ir ${CMAKE_CURRENT_SOURCE_DIR}/main.asm 0
	DEPENDS asm2ir
)