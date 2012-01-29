#ifndef YAJP_MAPPERS_ANYMAPPER_HPP
#define YAJP_MAPPERS_ANYMAPPER_HPP

#include <boost/any.hpp>
#include <vector>
#include <map>
#include <string>

namespace yajp {
namespace mappers {

class AnyMapper {
public:
    typedef std::vector<boost::any> Array;
    typedef std::map<std::string, boost::any> Object;
private:
    boost::any topValue;
    std::list<boost::any*> _stack; // parent objects we're working on and what not
    boost::any& currentValue;
    std::string lastPropertyName;
public:
    AnyMapper() : currentValue(topValue) {
        _stack.push_back(&topValue);
    }
    boost::any& getValue() { return topValue; }
    template <typename T>
    void foundSimpleValue(T value) {
        if (currentValue.type() == typeid(void)) {
            // If it's a simple value just set it
            topValue = value;
            currentValue = topValue;
        } else if (currentValue.type() == typeid(Array)) {
            // If current value is a array .. append
            Array& array = boost::any_cast<Array&>(currentValue);
            array.push_back(value);
        } else if (currentValue.type() == typeid(Object)) {
            // If we're reading a property .. set it in the map
            Object& object = boost::any_cast<Object&>(currentValue);
            object.insert(std::make_pair(lastPropertyName, value));
            lastPropertyName = "";
        }
    }
    void foundNull() {
        foundSimpleValue(boost::any());
    }
    template <class T>
    void startArrayOrObject() {
        if (currentValue.type() == typeid(void)) {
            // If it's a simple value just set it
            topValue = T();
            currentValue = topValue;
        } else if (currentValue.type() == typeid(Array)) {
            // If current value is an array .. append
            Array& array = boost::any_cast<Array&>(currentValue);
            array.push_back(T());
            currentValue = array.back();
        } else if (currentValue.type() == typeid(Object)) {
            // If we're reading a property .. set it in the map
            Object& object = boost::any_cast<Object&>(currentValue);
            currentValue = object[lastPropertyName]; // Get a refernce to the empty 'any'
            currentValue = T(); // Initialize it with what we're going to use it for (Array/Object)
        }
        _stack.push_back(&currentValue);
    }
    void startArray() {
        startArrayOrObject<Array>();
    }
    void endArray() {
        _stack.pop_back();
        currentValue = *_stack.back();
    }
    void startObj() {
        startArrayOrObject<Object>();
    }
    void endObj() {
        endArray();
    }
    void propertyName(std::string&& name) {
        lastPropertyName = std::move(name);
    }
};

} // namespace mappers
} // namespace yajp

#endif // YAJP_MAPPERS_ANYMAPPER_HPP
