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

#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <parser/json.hpp>

BOOST_AUTO_TEST_SUITE( string )

BOOST_AUTO_TEST_CASE( empty_string ) {
    std::string json("\"\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("", parser.readString());
}

BOOST_AUTO_TEST_CASE( simple_string ) {
    std::string json("\"ABC\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("ABC", parser.readString());
}

BOOST_AUTO_TEST_CASE( carriage  ) {
    std::string json("\"x\\ry\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("x\ry", parser.readString());
}

BOOST_AUTO_TEST_CASE( back_slash  ) {
    std::string json("\"one \\\\ two\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("one \\ two", parser.readString());
}

BOOST_AUTO_TEST_CASE( quote_start ) {
    std::string json("\"\\\"This is a quote.\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("\"This is a quote.", parser.readString());
}

BOOST_AUTO_TEST_CASE( quote_middle ) {
    std::string json("\"This is a \\\"quote\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("This is a \"quote", parser.readString());
}

BOOST_AUTO_TEST_CASE( quote_end ) {
    std::string json("\"This is a quote\\\"\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("This is a quote\"", parser.readString());
}

BOOST_AUTO_TEST_CASE( quote_three ) {
    std::string json("\"\\\"This is a \\\"quote\\\"\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("\"This is a \"quote\"", parser.readString());
}

BOOST_AUTO_TEST_CASE( lots_of_specials1 ) {
    std::string json("\"This is a \\\"quote. A backslash \\\\. A front slash /. A formfeed \\f. A new line \\n. a carriage return \\r.BC\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL("This is a \"quote. A backslash \\. A front slash /. A formfeed \f. A new line \n. a carriage return \r.BC", parser.readString());
}

BOOST_AUTO_TEST_CASE( unicode_cents ) {
    std::string json("\"This is \\u00A2 unicode\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL(u8"This is \u00A2 unicode", parser.readString());
}

BOOST_AUTO_TEST_CASE( unicode_euro ) {
    std::string json("\"This is \\u20aC unicode\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL(u8"This is \u20ac unicode", parser.readString());
}

BOOST_AUTO_TEST_CASE( unicode_square ) {
    std::string json("\"This is \\u20aC unicode\"");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL(u8"This is \u20ac unicode", parser.readString());
}

BOOST_AUTO_TEST_CASE( junk_after ) {
    std::string json(" \"name\" : junkyStuff");
    yajp::JSONParser parser(json);
    BOOST_CHECK_EQUAL(parser.getNextType(), yajp::JSONParser::string);
    BOOST_CHECK_EQUAL(u8"name", parser.readString());
}

BOOST_AUTO_TEST_SUITE_END()
