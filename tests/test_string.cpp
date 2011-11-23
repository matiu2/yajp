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
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "../string.hpp"

struct StringCallBackHandler {
    std::string output;
    void foundString(std::string&& input) {
        output = input;
    }
};

BOOST_FIXTURE_TEST_SUITE( json_string, StringCallBackHandler)

BOOST_AUTO_TEST_CASE( empty_string ) {
    yajp::parseString("\"\"", *this);
    BOOST_CHECK_EQUAL("", this->output);
}

BOOST_AUTO_TEST_CASE( simple_string ) {
    yajp::parseString("\"ABC\"", *this);
    BOOST_CHECK_EQUAL("ABC", this->output);
}

BOOST_AUTO_TEST_CASE( carriage  ) {
    yajp::parseString("\"x\\ry\"", *this);
    BOOST_CHECK_EQUAL("x\ry", this->output);
}

BOOST_AUTO_TEST_CASE( back_slash  ) {
    yajp::parseString("\"one \\\\ two\"", *this);
    BOOST_CHECK_EQUAL("one \\ two", this->output);
}

BOOST_AUTO_TEST_CASE( quote_start ) {
    yajp::parseString("\"\\\"This is a quote.\"", *this);
    BOOST_CHECK_EQUAL("\"This is a quote.", this->output);
}

BOOST_AUTO_TEST_CASE( quote_middle ) {
    yajp::parseString("\"This is a \\\"quote\"", *this);
    BOOST_CHECK_EQUAL("This is a \"quote", this->output);
}

BOOST_AUTO_TEST_CASE( quote_end ) {
    yajp::parseString("\"This is a quote\\\"\"", *this);
    BOOST_CHECK_EQUAL("This is a quote\"", this->output);
}

BOOST_AUTO_TEST_CASE( quote_three ) {
    yajp::parseString("\"\\\"This is a \\\"quote\\\"\"", *this);
    BOOST_CHECK_EQUAL("\"This is a \"quote\"", this->output);
}

/*
BOOST_AUTO_TEST_CASE( lots_of_specials1 ) {
    yajp::parseString("\"This is a \\\"quote. A backslash \\\\. A front slash \\/. A formfeed \\f. A new line \\n. a carriage return \\r.BC\"", *this);
    BOOST_CHECK_EQUAL("This is a \"quote. A backslash \\. A front slash /. A formfeed \f. A new line \n. a carriage return \r.BC", this->output);
}
*/
BOOST_AUTO_TEST_SUITE_END()
