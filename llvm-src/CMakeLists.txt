add_executable(llvm_asm_app main.cpp)

target_include_directories(llvm_asm_app PRIVATE ${LLVM_INCLUDE_DIRS})

target_link_libraries(llvm_asm_app PRIVATE LLVM sfml_api)

target_include_directories(llvm_asm_app PRIVATE ${PROJECT_ROOT}/src/api)

add_custom_target(run_llvm_app
	${CMAKE_CURRENT_BINARY_DIR}/llvm_asm_app
	DEPENDS llvm_asm_app
)