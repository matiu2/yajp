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
    virtual const char* type() { return "Person"; }
    std::string name;
    unsigned short age;
};

struct Employee : public Person {
    std::vector<std::string> departments;
};

struct Manager : public Employee {
    std::vector<Employee> team;
};

namespace {
    using yajp::JSONParser;
    using yajp::mappers::JSONValueMapper;
    using boost::any_cast;
};

BOOST_AUTO_TEST_SUITE( json_value )

BOOST_AUTO_TEST_CASE( simpleMap ) {
    // Parse the json
    std::string json2parse = R"( { "name": "tweedle", "age": 5})";
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

BOOST_AUTO_TEST_CASE( employee ) {
    // Parse the json
    std::string json2parse = R"( { "name": "mister bob", "age": 50, "departments": ["art", "science"] } )";
    JSONParser parser(json2parse);
    JSONValueMapper mapper;
    parser.parseJSONValue(mapper);
    yajp::JSONObject* result = dynamic_cast<yajp::JSONObject*>(mapper.getValue());
    assert(result != 0);
    // Now turn the result into a person
    Employee bob;
    bob.name = result->getString("name");
    bob.age = result->getNumber<unsigned long>("age");
    yajp::JSONArray* departments = result->getArray("departments");
    BOOST_ASSERT_MSG(departments, "Couldn't read any departments for employee bob");
    for (yajp::JSONArray::iterator dept=departments->begin(); dept!=departments->end(); ++dept) {
        yajp::JSONString* s = dynamic_cast<yajp::JSONString*>(dept->get());
        BOOST_ASSERT_MSG(s, "Array didn't have a string in it");
        bob.departments.push_back(std::move(s->asString()));
    }
    // Check  that we did it right
    BOOST_CHECK_EQUAL(bob.name, "mister bob");
    BOOST_CHECK_EQUAL(bob.age, 50);
    BOOST_REQUIRE_EQUAL(bob.departments.size(), 2);
    BOOST_CHECK_EQUAL(bob.departments[0], "art");
    BOOST_CHECK_EQUAL(bob.departments[1], "science");
}

BOOST_AUTO_TEST_CASE( manager ) {
    // Parse the json
    std::string json2parse = R"( { "name": "The Boss", "age": 30, "departments": ["fun", "golf"],
        "team":[{
                "age": 32,
                "name": "underling 1",
                "departments": ["serving", "licking boots"]
            },
            {
                "age": 13,
                "name": "underling 2",
                "departments": ["filing", "kissing butt"]
            }
        ]}
    )";
    JSONParser parser(json2parse);
    JSONValueMapper mapper;
    parser.parseJSONValue(mapper);
    yajp::JSONObject* result = dynamic_cast<yajp::JSONObject*>(mapper.getValue());
    assert(result != 0);
    // Now turn the result into a person
    auto readEmployee = [](Employee* p, yajp::JSONObject* data) {
        p->name = data->getString("name");
        p->age = data->getNumber<unsigned long>("age");
        yajp::JSONArray* departments = data->getArray("departments");
        BOOST_ASSERT_MSG(departments, (std::string("Couldn't read any departments for person:") + p->name).c_str());
        for (yajp::JSONArray::iterator dept=departments->begin(); dept!=departments->end(); ++dept) {
            yajp::JSONString* s = dynamic_cast<yajp::JSONString*>(dept->get());
            BOOST_ASSERT_MSG(s, "Array of departments didn't have a string in it");
            p->departments.push_back(std::move(s->asString()));
        }
    };
    Manager boss;
    readEmployee(dynamic_cast<Employee*>(&boss), result);
    yajp::JSONArray* teamData = result->getArray("team");
    BOOST_ASSERT_MSG(teamData, "Couldn't find a team");
    for (yajp::JSONArray::const_iterator member=teamData->begin();member!=teamData->end();++member) {
        Employee p;
        readEmployee(&p, dynamic_cast<yajp::JSONObject*>(member->get()));
        boss.team.push_back(p);
    }
    // Check  that we did it right
    BOOST_CHECK_EQUAL(boss.name, "The Boss");
    BOOST_CHECK_EQUAL(boss.age, 30);
    BOOST_REQUIRE_EQUAL(boss.departments.size(), 2);
    BOOST_CHECK_EQUAL(boss.departments[0], "fun");
    BOOST_CHECK_EQUAL(boss.departments[1], "golf");
    BOOST_REQUIRE_EQUAL(boss.team.size(), 2);
    const Employee& u1 = boss.team[0];
    BOOST_CHECK_EQUAL(u1.name, "underling 1");
    BOOST_CHECK_EQUAL(u1.age, 32);
    BOOST_REQUIRE_EQUAL(u1.departments.size(), 2);
    BOOST_CHECK_EQUAL(u1.departments[0], "serving");
    BOOST_CHECK_EQUAL(u1.departments[1], "licking boots");
    const Employee& u2 = boss.team[1];
    BOOST_CHECK_EQUAL(u2.name, "underling 2");
    BOOST_CHECK_EQUAL(u2.age, 13);
    BOOST_REQUIRE_EQUAL(u2.departments.size(), 2);
    BOOST_CHECK_EQUAL(u2.departments[0], "filing");
    BOOST_CHECK_EQUAL(u2.departments[1], "kissing butt");
}


BOOST_AUTO_TEST_SUITE_END()
