add_subdirectory(api)

add_executable(game_of_life main.c)
target_link_libraries(game_of_life PRIVATE sfml_api)
set_property(TARGET game_of_life PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_ROOT})

add_custom_target(run_app COMMAND ${PROJECT_BINARY_ROOT}/game_of_life)
add_dependencies(run_app game_of_life)

add_executable(managed_game_of_life main.c ${LOGGER_FILE})
add_dependencies(managed_game_of_life Logger)
target_compile_options(managed_game_of_life PRIVATE
					   "SHELL:-Xclang -load"
					   "SHELL:-Xclang ${PROJECT_BINARY_ROOT}/llvm-pass/libLogger.so"
					   -flegacy-pass-manager
)

target_link_libraries(managed_game_of_life PRIVATE sfml_api)
set_property(TARGET managed_game_of_life PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_ROOT})

add_custom_target(run_managed_app COMMAND ${PROJECT_BINARY_ROOT}/managed_game_of_life)
add_dependencies(run_managed_app managed_game_of_life)

add_custom_target(
		run-get-log
		COMMAND timeout 30 ${PROJECT_BINARY_ROOT}/managed_game_of_life > ${PROJECT_BINARY_ROOT}/log.txt
		DEPENDS managed_game_of_life
)

add_custom_target(
		run-get-statistics
		COMMAND ${Python3_EXECUTABLE} ${PROJECT_SCRIPTS}/get_statistics.py ${PROJECT_BINARY_ROOT}/log.txt ${PROJECT_ROOT}/pictures
		DEPENDS run-get-log
)
