project(speed_tests)

add_definitions( -O3 -march=native )

add_executable(number_yajp test_number_speed.cpp)
add_executable(number_atof test_atof.cpp)
