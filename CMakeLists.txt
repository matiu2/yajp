project(yajp)

cmake_minimum_required(VERSION 2.8)

add_custom_target(ragel_images ALL
    COMMAND ragel -Vp ${CMAKE_CURRENT_SOURCE_DIR}/number.rl | dot -Tpng > number.png
)

add_custom_target(number_hpp ALL
    COMMAND ragel -G2 number.rl -o number.hpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES number.rl
)

if ( ${CMAKE_BUILD_TYPE} STREQUAL "Debug" )
    add_definitions( -D DEBUG )
endif()

add_executable(demo_number demo_number.cpp)
add_dependencies(demo_number number_hpp)

add_subdirectory(tests)
