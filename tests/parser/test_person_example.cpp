/*
 * =====================================================================================
 *
 *       Filename:  test_json.cpp
 *
 *    Description:  Test cases for json
 *
 *        Version:  1.0
 *        Created:  27/11/11 03:05:49
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
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/any.hpp>
#include <parser/json.hpp>

typedef boost::any JSONValue;
typedef std::map<std::string, JSONValue> JSONObj;
typedef std::vector<JSONValue> JSONArray;

using boost::any_cast;

/// An example object that we'll be reading .. a person
struct Person {
    std::string name;
    unsigned short age;
    Person () : name(""), age(0) {}
};

class PersonReadFailure : std::runtime_error {
private:
    static std::string prettyMessage(const yajp::JSONParserError& e) {
        return std::string("Failed to read a person. JSON was:\n") +
               (e.parser()->json()) + '\n' +
               std::string((e.location() - e.parser()->json()), '-') + "^\n";
    }
public:
    PersonReadFailure(const yajp::JSONParserError& e) : std::runtime_error(prettyMessage(e)) {}
    PersonReadFailure(const char* msg) : std::runtime_error(msg) {}
    PersonReadFailure(const std::string& msg) : std::runtime_error(msg) {}
};

/// This is an example of how one could parse some json code to an object in real life
void readPerson(yajp::JSONParser& json, Person& person) {
    if (json.getNextType() != yajp::JSONParser::object)
        throw PersonReadFailure("This isn't an object");
    typedef yajp::JSONParser::JSONType JSONType;
    std::map<std::string, std::pair<JSONType, std::function<void()>>> map = {
        {"name", {
            JSONType::string,
            [&json, &person]() { person.name = json.readString(); }}},
        {"age" , {
            JSONType::number,
            [&json, &person]() { person.age = json.readNumber<short>(); }}}
    };
    size_t gotAttrs = 0;
    try {
        do {
            std::string nextAttrName = json.readNextAttribute();
            // See if we expected an attribute with that name
            auto pReader = map.find(nextAttrName);
            if (pReader == map.end()) {
                json.consumeOneValue();
                continue;
            }
            // See if the type matches what we expected
            JSONType nextTokenType = json.getNextType();
            auto pair = pReader->second;
            auto expectedType = pair.first;
            auto reader = pair.second;
            if (nextTokenType != expectedType) {
                std::stringstream errMsg;
                errMsg << "When reading attribute "
                       << nextAttrName
                       << "I expected a value of type "
                       << expectedType
                       << "but got one of type "
                       << nextTokenType;
                throw PersonReadFailure(errMsg.str());
            }
            reader(); // Actually read in the value to the person object
            ++gotAttrs;
            // Skip til the next post
        } while (json.doIHaveMoreObject());
        if (gotAttrs < map.size())
            throw PersonReadFailure("Couldn't find all the attributes");
    } catch (yajp::JSONParserError e) {
        throw PersonReadFailure(e);
    }
}

BOOST_AUTO_TEST_SUITE( person_example )

/// Tests that the expample fails to read a person from an empty string
BOOST_AUTO_TEST_CASE( empty_string ) {
    yajp::JSONParser parser("\"\"");
    Person result;
    BOOST_REQUIRE_THROW(readPerson(parser, result), PersonReadFailure);
    BOOST_CHECK_EQUAL(result.name, "");
    BOOST_CHECK_EQUAL(result.age, 0);
}

//// Tests that the example correctly reads a person
BOOST_AUTO_TEST_CASE( read_a_person ) {
    std::string json = R"( { "name": "tweedle", "age": 5 } )";
    yajp::JSONParser parser(json);
    Person result;
    readPerson(parser, result);
    BOOST_CHECK_EQUAL("tweedle", result.name);
    BOOST_CHECK_EQUAL(5, result.age);
}

/// Tests that the example fails if there is only one attribute
BOOST_AUTO_TEST_CASE( only_one_attr ) {
    std::string json(R"( { "age": 38 } )");
    yajp::JSONParser parser(json);
    Person result;
    BOOST_REQUIRE_THROW(readPerson(parser, result), PersonReadFailure);
    BOOST_CHECK_EQUAL("", result.name);
    BOOST_CHECK_EQUAL(38, result.age);
}

/// Tests that extra attributes are ignored
BOOST_AUTO_TEST_CASE( ignore_junk ) {
    std::string json(R"( { "job": "Stunt man", "name": "Mister Jumpy", "heart-rate": 190.2, "age": 38 } )");
    yajp::JSONParser parser(json);
    Person result;
    readPerson(parser, result);
    BOOST_CHECK_EQUAL("Mister Jumpy", result.name);
    BOOST_CHECK_EQUAL(38, result.age);
}

BOOST_AUTO_TEST_SUITE_END()
