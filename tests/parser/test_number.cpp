/*
 * =====================================================================================
 *
 *       Filename:  test_number.cpp
 *
 *    Description:  Tests the number parsing powerz
 *
 *        Version:  1.0
 *        Created:  14/11/11 13:38:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <parser/json.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( number )

BOOST_AUTO_TEST_CASE( zero ) {
    yajp::JSONParser json("0");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    int result = json.readNumber<int>();
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE( one ) {
    yajp::JSONParser json("1");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    int result = json.readNumber<int>();
    BOOST_CHECK_EQUAL(result, 1);
}

BOOST_AUTO_TEST_CASE( hundreds ) {
    yajp::JSONParser json("234");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    int result = json.readNumber<int>();
    BOOST_CHECK_EQUAL(result, 234);
}

BOOST_AUTO_TEST_CASE( large_number ) {
    yajp::JSONParser json("234989869123123123");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    unsigned long long result = json.readNumber<unsigned long long>();
    BOOST_CHECK_EQUAL(result, 234989869123123123);
}

BOOST_AUTO_TEST_CASE( negative_zero ) {
    yajp::JSONParser json("-0");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    int result = json.readNumber<int>();
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE( negative_zero_point_zero ) {
    yajp::JSONParser json("-0.0");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    double result = json.readNumber();
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE( zero_point_zero ) {
    yajp::JSONParser json("0.0");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    double result = json.readNumber();
    BOOST_CHECK_EQUAL(result, 0);
}

BOOST_AUTO_TEST_CASE( one_negative_exponent ) {
    yajp::JSONParser json("1e-1");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    double result = json.readNumber();
    BOOST_CHECK_EQUAL(result, 0.1);
}

BOOST_AUTO_TEST_CASE( positive_exponent_int ) {
    yajp::JSONParser json("1e10");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    double result = json.readNumber();
    BOOST_CHECK_EQUAL(result, 10000000000);
}

BOOST_AUTO_TEST_CASE( float_to_int_with_exponent ) {
    yajp::JSONParser json("0.0321e10");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    double result = json.readNumber();
    BOOST_CHECK_EQUAL(result, 321000000);
}

BOOST_AUTO_TEST_CASE( neg_float_to_int_with_exponent ) {
    yajp::JSONParser json("-0.0321e10");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    double result = json.readNumber();
    BOOST_CHECK_EQUAL(result, -321000000);
}

BOOST_AUTO_TEST_CASE( neg_neg ) {
    yajp::JSONParser json("-0.0321e-10");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    double result = json.readNumber();
    BOOST_CHECK_CLOSE(result, -0.00000000000321, 10);
}

BOOST_AUTO_TEST_CASE( long_double ) {
    yajp::JSONParser json("3.141592653589793");
    BOOST_REQUIRE_EQUAL(json.getNextType(), yajp::JSONParser::number);
    double result = json.readNumber();
    BOOST_CHECK_CLOSE(result, 3.141592653589793, 10);
}

BOOST_AUTO_TEST_SUITE_END()
