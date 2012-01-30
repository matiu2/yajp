/*
 * =====================================================================================
 *
 *       Filename:  test_any.cpp
 *
 *    Description:  Test the JSONValue mapper
 *
 *        Version:  1.0
 *        Created:  25/01/12 13:38:15
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

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/any.hpp>
#include "../parser/json.hpp"
#include "../mappers/JSONValueMapper.hpp"

/// An example object that we'll be reading .. a person
struct Person {
    std::string name;
    unsigned short age;
    template<class T>
    void jsonify(T& translator) {
        translator.addField("name", name);
        translator.addField("age", age);
    }
};

BOOST_AUTO_TEST_SUITE( json_value )

BOOST_AUTO_TEST_CASE( simpleMap ) {
    using yajp::JSONParser;
    using yajp::mappers::JSONValueMapper;
    using boost::any_cast;
    // Parse the json
    std::string json2parse = R"( { "name": "tweedle", "age": 5 )";
    JSONParser parser(json2parse);
    JSONValueMapper mapper;
    parser.parseJSONValue(mapper);
    yajp::JSONObject* result = dynamic_cast<yajp::JSONObject*>(mapper.getValue());
    assert(result != 0);
    // Now turn the result into a person
    Person p;
    p.name = result->getString("name");
    p.age = result->getNumber<unsigned long>("age");
    BOOST_CHECK_EQUAL(p.name, "tweedle");
    BOOST_CHECK_EQUAL(p.age, 5);
}
BOOST_AUTO_TEST_SUITE_END()
