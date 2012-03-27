project(yajp)

cmake_minimum_required(VERSION 2.8)

add_definitions( -std=c++11 -Wall -Wextra )

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    add_definitions( -D DEBUG )
endif()

# Tests
set( BUILD_TESTS true CACHE bool "Build tests so you can run: make test" )
if (${BUILD_TESTS})
    enable_testing()
endif()

add_subdirectory(parser)

add_subdirectory(tests)

add_subdirectory(speed_tests)

add_executable(demo_number demo_number.cpp)

