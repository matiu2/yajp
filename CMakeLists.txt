project(yajp)

cmake_minimum_required(VERSION 2.8)

add_definitions( -std=c++0x -Wall -Wextra )

add_custom_target(ragel_images
    COMMAND ragel -Vp ${CMAKE_CURRENT_SOURCE_DIR}/number_standalone.rl | dot -Tpng > number.png
    COMMAND ragel -Vp "${CMAKE_CURRENT_SOURCE_DIR}/string_standalone.rl" | dot -Tpng > "string.png"
    COMMAND ragel -Vp "${CMAKE_CURRENT_SOURCE_DIR}/json.rl" | dot -Tpng > "json.png"
)

add_custom_target(
    ragel_headers
    COMMAND ragel -G0 "${CMAKE_CURRENT_SOURCE_DIR}/number_standalone.rl" -o "${CMAKE_CURRENT_SOURCE_DIR}/number.hpp"
    COMMAND ragel -G0 "${CMAKE_CURRENT_SOURCE_DIR}/string_standalone.rl" -o "${CMAKE_CURRENT_SOURCE_DIR}/string.hpp"
    COMMAND ragel -G0 "${CMAKE_CURRENT_SOURCE_DIR}/json.rl" -o "${CMAKE_CURRENT_SOURCE_DIR}/json.hpp"
    DEPENDS number.rl string.rl json.rl
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
