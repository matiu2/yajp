project(yajp)

cmake_minimum_required(VERSION 2.8)

add_definitions( -std=c++0x -Wall -Wextra )

add_custom_target(ragel_images ALL
    COMMAND ragel -Vp ${CMAKE_CURRENT_SOURCE_DIR}/number.rl | dot -Tpng > number.png
    COMMAND ragel -Vp "${CMAKE_CURRENT_SOURCE_DIR}/string.rl" | dot -Tpng > "string.png"
)

add_custom_target(
    ragel_headers
    COMMAND ragel -L -G1 number.rl -o number.hpp
    COMMAND ragel -L -G1 "string.rl" -o "string.hpp"
    DEPENDS number.rl string.rl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    add_definitions( -D DEBUG )
endif()

add_executable(demo_number demo_number.cpp)

# Tests
set( BUILD_TESTS true CACHE bool "Build tests so you can run: make test" )
if (${BUILD_TESTS})
    enable_testing()
endif()

add_subdirectory(tests)

add_subdirectory(speed_tests)
