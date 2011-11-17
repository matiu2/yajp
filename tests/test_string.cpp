/*
 * =====================================================================================
 *
 *       Filename:  test_string.cpp
 *
 *    Description:  Tests the string parser part of the state machine
 *
 *        Version:  1.0
 *        Created:  16/11/11 19:32:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "JSON String Module"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "../string.hpp"

struct StringCallBackHandler {
    std::string output;
    void foundString(std::string&& input) {
        output = input;
    }
};

BOOST_AUTO_TEST_CASE( empty_string ) {
    StringCallBackHandler cb;
    yajp::parseString("\"\"", cb);
    BOOST_CHECK_EQUAL("", cb.output);
}

BOOST_AUTO_TEST_CASE( simple_string ) {
    StringCallBackHandler cb;
    yajp::parseString("\"ABC\"", cb);
    BOOST_CHECK_EQUAL("ABC", cb.output);
}
