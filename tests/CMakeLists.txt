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
        ADD_TEST(${TEST} ${EXE_NAME} --run_test=${TEST})
    endforeach()
endfunction()

boost_test(number
    zero
    one
    millions
    negative_zero_point_zero
    one_negative_exponent
    float_to_int_with_exponent
    neg_neg
)

# For each test library copy and paste and change this

boost_test("string"
    empty_string
    simple_string
    carriage
    back_slash
    quote_start
    quote_middle
    quote_end
    quote_three
    lots_of_specials1
)
