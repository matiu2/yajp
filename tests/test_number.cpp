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

#include <boost/test/unit_test.hpp>
#include "../number.hpp"

struct CallbackHandler {
    enum LastCall {none, ul, l, d} lc; // just used to know which one was called
    unsigned long ulVal;
    long lVal;
    double dVal;
    void foundSimpleValue(unsigned long num) {
        lc = ul;
        ulVal = num;
    }
    void foundSimpleValue(long num) {
        lc = l;
        lVal = num;
    }
    void foundSimpleValue(double num) {
        lc = d;
        dVal = num;
    }
    CallbackHandler() : lc(none) {}
};

BOOST_FIXTURE_TEST_SUITE( number, CallbackHandler )

BOOST_AUTO_TEST_CASE( zero ) {
    yajp::parseNumber("0", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(ulVal, 0);
}

BOOST_AUTO_TEST_CASE( one ) {
    yajp::parseNumber("1", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(ulVal, 1);
}

BOOST_AUTO_TEST_CASE( hundreds ) {
    yajp::parseNumber("234", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(ulVal, 234);
}

BOOST_AUTO_TEST_CASE( millions ) {
    yajp::parseNumber("234989869", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(ulVal, 234989869);
}

BOOST_AUTO_TEST_CASE( negative_zero ) {
    yajp::parseNumber("-0", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::l);
    BOOST_CHECK_EQUAL(lVal, 0);
}

BOOST_AUTO_TEST_CASE( negative_zero_point_zero ) {
    yajp::parseNumber("-0.0", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::d);
    BOOST_CHECK_EQUAL(dVal, 0);
}

BOOST_AUTO_TEST_CASE( zero_point_zero ) {
    yajp::parseNumber("0.0", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::d);
    BOOST_CHECK_EQUAL(dVal, 0);
}

BOOST_AUTO_TEST_CASE( one_negative_exponent ) {
    yajp::parseNumber("1e-1", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::d);
    BOOST_CHECK_EQUAL(dVal, 0.1);
}

BOOST_AUTO_TEST_CASE( positive_exponent_int ) {
    yajp::parseNumber("1e10", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(ulVal, 10000000000);
}

BOOST_AUTO_TEST_CASE( float_to_int_with_exponent ) {
    yajp::parseNumber("0.0321e10", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(ulVal, 321000000);
}

BOOST_AUTO_TEST_CASE( neg_float_to_int_with_exponent ) {
    yajp::parseNumber("-0.0321e10", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::l);
    BOOST_CHECK_EQUAL(lVal, -321000000);
}

BOOST_AUTO_TEST_CASE( neg_neg ) {
    yajp::parseNumber("-0.0321e-10", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::d);
    BOOST_CHECK_CLOSE(dVal, -0.00000000000321, 10);
}

BOOST_AUTO_TEST_CASE( long_double ) {
    yajp::parseNumber("3.141592653589793", *this);
    BOOST_CHECK_EQUAL(lc, CallbackHandler::d);
    BOOST_CHECK_CLOSE(dVal, 3.141592653589793, 10);
}

BOOST_AUTO_TEST_SUITE_END()
