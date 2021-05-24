cmake_minimum_required (VERSION 3.13)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project (LIFE)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

if (NOT TARGET PGE)
	find_package (PGE)
endif ()

add_definitions ("-DUNICODE")
add_definitions ("-D_UNICODE")

include_directories ("${PROJECT_SOURCE_DIR}/Headers")

add_executable (${PROJECT_NAME} LIFE.cpp)
target_link_libraries (${PROJECT_NAME} PRIVATE PGE)
