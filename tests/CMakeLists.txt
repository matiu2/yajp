project(tests)

ADD_DEFINITIONS( -fno-access-control ) # Helps us access private methods in functions

# For each test library copy and paste and change this
ADD_LIBRARY(numberTest SHARED test_number.cpp)
SET(TEST_LIBS numberTest)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

ADD_EXECUTABLE(test_runner main.cpp)
TARGET_LINK_LIBRARIES(test_runner ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${TEST_LIBS})

ADD_TEST(testNumberParsing test_runner)
