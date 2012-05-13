/*
 * =====================================================================================
 *
 *       Filename:  test_any.cpp
 *
 *    Description:  Test the boost::any mapper
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
#include "../mappers/AnyMapper.hpp"

/// An example object that we'll be reading .. a person
struct Person {
    std::string name;
    unsigned short age;
};

/// An example object that we'll be reading .. a person
struct Person2 : public Person {
    std::vector<std::string> languages;
};

BOOST_AUTO_TEST_SUITE( any )

BOOST_AUTO_TEST_CASE( simpleMap ) {
    using yajp::JSONParser;
    using yajp::mappers::AnyMapper;
    using boost::any_cast;
    // Parse the json
    std::string json2parse = R"( { "name": "tweedle", "age": 5 } )";
    JSONParser parser(json2parse);
    AnyMapper mapper;
    parser.parseJSONValue(mapper);
    boost::any& result = mapper.getValue();
    // Now turn the result into a person
    Person p;
    AnyMapper::Object& jsonObj = boost::any_cast<AnyMapper::Object&>(result);
    p.name = std::move(any_cast<std::string&>(jsonObj["name"]));
    boost::any& age1 = jsonObj["age"];
    std::cout << age1.type().name() << std::endl;
    p.age = any_cast<unsigned long>(age1);
    BOOST_CHECK_EQUAL(p.name, "tweedle");
    BOOST_CHECK_EQUAL(p.age, 5);
}

BOOST_AUTO_TEST_CASE( arrayInMap ) {
    using yajp::JSONParser;
    using yajp::mappers::AnyMapper;
    using boost::any_cast;
    // Parse the json
    std::string json2parse = R"( { "name": "tweedle", "languages": ["en", "es"], "age": 5 } )";
    JSONParser parser(json2parse);
    AnyMapper mapper;
    parser.parseJSONValue(mapper);
    boost::any& result = mapper.getValue();
    // Now turn the result into a person
    Person2 p;
    AnyMapper::Object& jsonObj = boost::any_cast<AnyMapper::Object&>(result);
    // name
    p.name = std::move(any_cast<std::string&>(jsonObj["name"]));
    // age
    boost::any& age1 = jsonObj["age"];
    p.age = any_cast<unsigned long>(age1);
    // languages
    for (const boost::any& language: boost::any_cast<const yajp::mappers::AnyMapper::Array&>(jsonObj["languages"]))
        p.languages.push_back(boost::any_cast<const std::string&>(language));
    BOOST_CHECK_EQUAL(p.name, "tweedle");
    BOOST_CHECK_EQUAL(p.age, 5);
    std::vector<std::string> goodLangs({"en", "es"});
    BOOST_CHECK_EQUAL_COLLECTIONS(p.languages.begin(), p.languages.end(), goodLangs.begin(), goodLangs.end());
}

BOOST_AUTO_TEST_SUITE_END()
