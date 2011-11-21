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
#define BOOST_TEST_MODULE "number"

#include <boost/test/unit_test.hpp>
#include "../number.hpp"

struct CallbackHandler {
    enum LastCall {none, ul, l, d} lc; // just used to know which one was called
    unsigned long ulVal;
    long lVal;
    double dVal;
    void foundNumber(unsigned long num) {
        lc = ul;
        ulVal = num;
    }
    void foundNumber(long num) {
        lc = l;
        lVal = num;
    }
    void foundNumber(double num) {
        lc = d;
        dVal = num;
    }
    CallbackHandler() : lc(none) {}
};

BOOST_AUTO_TEST_CASE( zero ) {
    CallbackHandler cb;
    yajp::parseNumber("0", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(cb.ulVal, 0);
}

BOOST_AUTO_TEST_CASE( one ) {
    CallbackHandler cb;
    yajp::parseNumber("1", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(cb.ulVal, 1);
}

BOOST_AUTO_TEST_CASE( hundreds ) {
    CallbackHandler cb;
    yajp::parseNumber("234", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(cb.ulVal, 234);
}

BOOST_AUTO_TEST_CASE( millions ) {
    CallbackHandler cb;
    yajp::parseNumber("234989869", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(cb.ulVal, 234989869);
}

BOOST_AUTO_TEST_CASE( negative_zero ) {
    CallbackHandler cb;
    yajp::parseNumber("-0", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::l);
    BOOST_CHECK_EQUAL(cb.lVal, 0);
}

BOOST_AUTO_TEST_CASE( negative_zero_point_zero ) {
    CallbackHandler cb;
    yajp::parseNumber("-0.0", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::d);
    BOOST_CHECK_EQUAL(cb.dVal, 0);
}

BOOST_AUTO_TEST_CASE( zero_point_zero ) {
    CallbackHandler cb;
    yajp::parseNumber("0.0", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::d);
    BOOST_CHECK_EQUAL(cb.dVal, 0);
}

BOOST_AUTO_TEST_CASE( one_negative_exponent ) {
    CallbackHandler cb;
    yajp::parseNumber("1e-1", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::d);
    BOOST_CHECK_EQUAL(cb.dVal, 0.1);
}

BOOST_AUTO_TEST_CASE( positive_exponent_int ) {
    CallbackHandler cb;
    yajp::parseNumber("1e10", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(cb.ulVal, 10000000000);
}

BOOST_AUTO_TEST_CASE( float_to_int_with_exponent ) {
    CallbackHandler cb;
    yajp::parseNumber("0.0321e10", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::ul);
    BOOST_CHECK_EQUAL(cb.ulVal, 321000000);
}

BOOST_AUTO_TEST_CASE( neg_float_to_int_with_exponent ) {
    CallbackHandler cb;
    yajp::parseNumber("-0.0321e10", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::l);
    BOOST_CHECK_EQUAL(cb.lVal, -321000000);
}

BOOST_AUTO_TEST_CASE( neg_neg ) {
    CallbackHandler cb;
    yajp::parseNumber("-0.0321e-10", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::d);
    BOOST_CHECK_CLOSE(cb.dVal, -0.00000000000321, 10);
}

BOOST_AUTO_TEST_CASE( longlong ) {
    CallbackHandler cb;
    yajp::parseNumber("3.141592653589793", cb);
    BOOST_CHECK_EQUAL(cb.lc, CallbackHandler::d);
    BOOST_CHECK_CLOSE(cb.dVal, 3.141592653589793, 10);
}
