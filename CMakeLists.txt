project(yajp)

cmake_minimum_required(VERSION 2.8)

add_definitions( -std=c++11 -Wall )

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    add_definitions( -D DEBUG )
endif()

# Tests
set( BUILD_TESTS true CACHE bool "Build tests so you can run: make test" )
if (${BUILD_TESTS})
    enable_testing()
endif()

#set( BUILD_STATIC true CACHE bool "Build a static library instead of a shared one" )
set( BUILD_BOOST_ANY_MAPPER false CACHE bool "Adds dependency to boost::any - currently unavailable" ) # TODO: Add this
set( BUILD_JSON_TYPES false CACHE bool "Build the easy to use json types on top of the parser code - currently unavailable" ) # TODO: Add this

add_subdirectory(parser)

add_subdirectory(tests)

add_subdirectory(speed_tests)

add_executable(demo_number demo_number.cpp)

