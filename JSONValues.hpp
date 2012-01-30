/*
 * =====================================================================================
 *
 *       Filename:  JsonValue.hpp
 *
 *    Description:  Represents a json value. The top in a class hierachy
 *
 *        Version:  1.0
 *        Created:  30/01/12 08:59:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <map>
#include <vector>
#include <string>
#include <memory>

namespace yajp {

class JSONValue;

typedef std::unique_ptr<JSONValue> PJSONValue;

class JSONValue {
public:
    virtual const char* name() = 0;
};

template <typename NumType>
class JSONNumber : public JSONValue {
private:
    NumType value;
public:
    JSONNumber(NumType value) : value(value) {}
    NumType asNumber() { return value; }
    virtual const char* name() { return "Array"; }
};

class JSONString : public JSONValue {
private:
    std::string value;
public:
    virtual const char* name() { return "String"; }
    JSONString(const std::string& value="") : value(value) {}
    JSONString(std::string&& value) : value(std::move(value)) {}
    const std::string& asString() { return value; }
};


class JSONObject : public JSONValue, public std::map<std::string, PJSONValue> {
public:
    virtual const char* name() { return "Object"; }
    template <typename T> T getNumber(const std::string& name) {
        JSONValue* baseHolder = (*this)[name].get();
        JSONNumber<T>* holder = dynamic_cast<JSONNumber<T>*>(baseHolder);
        if (holder)
            return holder->asNumber();
        else
            throw std::logic_error(std::string("Couldn't find a number with the specified type with name: ") + name);
    }
    const std::string& getString(const std::string& name) {
        JSONString* holder = dynamic_cast<JSONString*>((*this)[name].get());
        if (holder)
            return holder->asString();
        else
            throw std::logic_error(std::string("Couldn't find a string value with name: ") + name);
    }
};

class JSONArray : public JSONValue, public std::vector<PJSONValue> {
public:
    virtual const char* name() { return "Array"; }
};

class JSONNull : public JSONValue {
public:
    virtual const char* name() { return "Null"; }
};


}
