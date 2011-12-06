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
#include "../json.hpp"

typedef boost::any JSONValue;
typedef std::map<std::string, JSONValue> JSONObj;
typedef std::vector<JSONValue> JSONArray;

using boost::any_cast;

struct JSONCallBackHandler {
    enum Reading {None, Obj, Array};
    Reading reading;
    JSONValue result;
    JSONValue* currentParent;
    std::vector<JSONValue*> stack;
    std::string property;
    JSONCallBackHandler() : reading(None), currentParent(&result), stack(1, &result) {}
    unsigned long objDepth;
    /// Called when the parser finds a simple value
    template <typename T>
    void foundSimpleValue(T value) {
        switch (reading) {
            case None:
                result = any_cast<T>(result);
                break;
            case Obj:
                any_cast<JSONObj>(*currentParent)[property] = value;
                break;
            case Array:
                any_cast<JSONArray>(*currentParent).push_back(value);
                break;
        };
    }
    void foundNull() { foundSimpleValue((long long)0); }
    void startArray() {
        JSONValue newArray = JSONArray();
        switch (reading) {
            case None:
                result = JSONValue(JSONArray());
                currentParent = &result;
                break;
            case Obj:
                any_cast<JSONObj>(*currentParent)[property] = newArray;
                stack.push_back(currentParent);
                currentParent = &newArray;
                break;
            case Array:
                JSONArray&& currentArray = any_cast<JSONArray>(*currentParent);
                currentArray.push_back(newArray);
                stack.push_back(currentParent);
                currentParent = &currentArray.back();
                break;
        }
        reading = Array;
    }
    void endArray() {
        currentParent = stack.back();
        stack.pop_back();
    }
    void startObj() {
        JSONValue newObj(JSONObj());
        switch (reading) {
            case None:
                result = newObj;
                currentParent = &result;
                break;
            case Obj:
                JSONObj* pCurrent = any_cast<JSONObj*>(currentParent);
                *pCurrent[property] = newObj;
                stack.push_back(currentParent);
                currentParent = &newObj;
                break;
            case Array:
                JSONArray* array = any_cast<JSONArray>(currentParent);
                array->push_back(newObj);
                stack.push_back(currentParent);
                currentParent = newObj;
                break;
         }
        reading = Obj;
    }
    void endObj() {
        currentParent = stack.back();
        stack.pop_back();
    }
    void propertyName(std::string&& name) { property = name; }
};

BOOST_FIXTURE_TEST_SUITE( json, JSONCallBackHandler)

BOOST_AUTO_TEST_CASE( empty_string ) {
    yajp::parseJSON("\"\"", *this);
    BOOST_CHECK_EQUAL("", any_cast<std::string>(result));
}

BOOST_AUTO_TEST_CASE( object_with_attributes ) {
    yajp::parseJSON(R"( { "name": "tweedle", "age": 5 )", *this);
    JSONObj& obj = any_cast<JSONObj>(result);
    BOOST_CHECK_EQUAL("tweedle", any_cast<std::string>(obj["name"]));
    BOOST_CHECK_EQUAL(5, any_cast<long>(obj["age"]));
}

BOOST_AUTO_TEST_SUITE_END()
