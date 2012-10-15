project(tests)

#ADD_DEFINITIONS( -fno-access-control ) # Helps us access private methods in functions

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# A Function for integrating BOOST.TEST and CTEST a little bit
# if MAIN_NAME is number:
#   source file will be: test_number.cpp
#   executable file will be: test_number
# Any other args are the tests to run on it
function(boost_test MAIN_NAME)
    set(EXE_NAME test_${MAIN_NAME})
    ADD_EXECUTABLE(${EXE_NAME} ${EXE_NAME}.cpp)
    TARGET_LINK_LIBRARIES(${EXE_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    FOREACH(TEST IN ITEMS ${ARGN})
        ADD_TEST(${MAIN_NAME}/${TEST} ${EXE_NAME} --run_test=${MAIN_NAME}/${TEST})
    endforeach()
endfunction()

get_filename_component(TO_INCLUDE ${CMAKE_SOURCE_DIR} ABSOLUTE)
include_directories(${TO_INCLUDE})
add_subdirectory(parser)
