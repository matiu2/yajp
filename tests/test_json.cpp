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
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/any.hpp>
#include "../parser/json.hpp"
#include "../mappers/Mapper.hpp"

typedef boost::any JSONValue;
typedef std::map<std::string, JSONValue> JSONObj;
typedef std::vector<JSONValue> JSONArray;

using boost::any_cast;

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

class PersonMapper : public yajp::mappers::Mapper {
private:
    Person& person;
    std::string lastPropertyName;
public:
    PersonMapper(Person& person) : person(person) {}
    void foundSimpleValue(std::string&& value) {
        assert(lastPropertyName == "name");
        person.name = value;
    }
    void foundSimpleValue(unsigned short value) {
        assert(lastPropertyName == "age");
        person.age = value;
    }
    void propertyName(std::string&& name) { lastPropertyName = name; }
};

BOOST_AUTO_TEST_SUITE( json )

BOOST_AUTO_TEST_CASE( empty_string ) {
    yajp::JSONParser parser("\"\"");
    std::string result;
    yajp::mappers::SimpleMapper<std::string> mapper(result);
    parser.parseJSONValue(mapper);
    BOOST_CHECK_EQUAL("", result);
}

BOOST_AUTO_TEST_CASE( read_a_person ) {
    yajp::JSONParser parser(R"( { "name": "tweedle", "age": 5 )");
    Person result;
    PersonMapper mapper(result);
    parser.parseJSONValue(mapper);
    BOOST_CHECK_EQUAL("tweedle", result.name);
    BOOST_CHECK_EQUAL(5, result.age);
}

BOOST_AUTO_TEST_SUITE_END()
