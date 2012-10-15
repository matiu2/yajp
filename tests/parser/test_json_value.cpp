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
#include <parser/json.hpp>
#include <yajp/mappers/JSONValueMapper.hpp>

/// An example object that we'll be reading .. a person
struct Person {
    virtual const char* type() { return "Person"; }
    std::string name;
    unsigned short age;
};

struct Employee : public Person {
    virtual const char* type() override { return "Employee"; }
    unsigned int number;
    std::vector<std::string> departments;
};

struct Manager : public Employee {
    virtual const char* type() override { return "Manager"; }
    std::vector<Employee> teamMembers;
};

BOOST_AUTO_TEST_SUITE( json_value )

BOOST_AUTO_TEST_CASE( simpleMap ) {
    // Parse the json
    yajp::JSONParser parser(R"( { "name": "tweedle", "age": 5 )");
    yajp::pJSONType parsed(yajp::parseToClasses(parser));
    yajp::pJSONObject result(dynamic_cast<yajp::JSONObject*>(parsed.release()));

    BOOST_REQUIRE(result);

    //yajp::JSONObject result = std::move(static_cast<yajp::JSONObject>()));
    // Now turn the result into a person
    Person p;
    p.name = result->getString("name");
    p.age = result->getNumber<unsigned long>("age");
    BOOST_CHECK_EQUAL(p.name, "tweedle");
    BOOST_CHECK_EQUAL(p.age, 5);
}

/// Helper function to read a person's data from json values
void readPerson(yajp::JSONObject* input, Person* output) {
    output->name = input->getString("name");
    output->age = input->getNumber<unsigned long>("age");
    Employee* emp = dynamic_cast<Employee*>(output);
    if (emp) {
        emp->departments = input->getVector("departments");
        Manager* man = dynamic_cast<Manager*>(emp);
        if (man != 0) {
            input->getVector<Employee>("team-members", man->teamMembers);
        }
    }
}

// Our own template specializations to convert JSON objects into real c++ objects
namespace yajp {

template <> Employee json2val<Employee>(yajp::JSONValue* val) {
    const std::string type = result->getString("type")
    if (type == "Manager") {
        Manager result;
        readPerson(val, &result);
        return result;
    } else {
        Employee result;
        readPerson(val, &result);
        return result;
    }
}

}


BOOST_AUTO_TEST_CASE( nestedMap ) {
    using yajp::JSONParser;
    using yajp::mappers::JSONValueMapper;
    using boost::any_cast;
    // Parse the json
    std::string json2parse = R"( { "type": "Manager" "name": "tweedle", "age": 5, "team-members": [ { "type":
    "Employee", "name": "dee", "age": 4, "departments": ["science", "marketing", "engineering"] }, { "type": "Manager",
    "name": "dum", "age": 2, "departments": ["fun", "games"] } ] } )";
    JSONParser parser(json2parse);
    JSONValueMapper mapper;
    parser.parseJSONValue(mapper);
    yajp::JSONObject* result = dynamic_cast<yajp::JSONObject*>(mapper.getValue());
    assert(result != 0);
    // Now turn the result into a person
    auto getPerson = [](yajp::JSONObject* value, Person* p){
        p->name = result->getString("name");
        p->age = result->getNumber<unsigned long>("age");
        Employee* emp = dynamic_cast<Employee*>(p);
        if (emp != 0) {
            value->readVector<string>("departments", emp->departments)
            Manager* man = dynamic_cast<Manager*>(p);
            if (man != 0) {
                value->getVector<Employee>("team-members", man->teamMembers);
            }
        }
    };
    auto getEmployee = [getPerson&](yajp::JSONObject* value,

    Person p;
    p.name = result->getString("name");
    BOOST_CHECK_EQUAL(p.name, "tweedle");
    BOOST_CHECK_EQUAL(p.age, 5);
}

BOOST_AUTO_TEST_SUITE_END()
