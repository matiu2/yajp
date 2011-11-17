project(yajp)

cmake_minimum_required(VERSION 2.8)

add_definitions( -std=c++0x -Wall )

add_custom_target(ragel_images ALL
    COMMAND ragel -Vp ${CMAKE_CURRENT_SOURCE_DIR}/number.rl | dot -Tpng > number.png
    COMMAND ragel -Vp ${CMAKE_CURRENT_SOURCE_DIR}/string.rl | dot -Tpng > string.png
)

add_custom_command(
    OUTPUT number.hpp
    COMMAND ragel -G2 number.rl -o number.hpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS number.rl
)

add_custom_command(
    OUTPUT string.hpp
    COMMAND ragel -G2 string.rl -o string.hpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES string.rl
)
if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    add_definitions( -D DEBUG )
endif()

add_executable(demo_number demo_number.cpp)
add_dependencies(demo_number "${CMAKE_CURRENT_SOURCE_DIR}/number.hpp")

# Tests
set( BUILD_TESTS true CACHE bool "Build tests so you can run: make test" )

if (${BUILD_TESTS})
    enable_testing()
endif()

add_subdirectory(tests)
add_subdirectory(speed_tests)
