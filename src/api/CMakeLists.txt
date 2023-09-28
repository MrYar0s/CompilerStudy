enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_SOURCES
	api.cpp
)

add_library(sfml_api STATIC ${LIBRARY_SOURCES})

target_link_libraries(sfml_api PRIVATE ${SFML_LIBS})