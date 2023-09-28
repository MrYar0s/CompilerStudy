add_subdirectory(api)

add_executable(game_of_life main.c)

target_link_libraries(game_of_life PRIVATE sfml_api)
set_property(TARGET game_of_life PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_ROOT})

add_custom_target(run_app COMMAND ${PROJECT_BINARY_ROOT}/game_of_life)
add_dependencies(run_app game_of_life)