/*
 * =====================================================================================
 *
 *       Filename:  JSONValueMapper.hpp
 *
 *    Description:  Maps the json parser callbacks into json_value/JSONValue classes
 *
 *        Version:  1.0
 *        Created:  30/01/12 09:16:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "../JSONValues.hpp"
#include "../json_exceptions.hpp"
#include <vector>
#include <string>

namespace yajp {
namespace mappers {

class JSONValueMapper {
private:
    PJSONValue root;
    std::vector<JSONValue*> stack;
    JSONObject* currentObject;
    JSONArray* currentArray;
    std::string lastPropertyName;
protected:
    template <typename T> PJSONValue newJSONNumber(T value) { return PJSONValue(new JSONNumber<T>(value)); }
    PJSONValue newJSONString(const std::string& value) { return PJSONValue(new JSONString(value)); }
    /// Puts a newly incoming value in the right place
    void storeValue(PJSONValue value) {
        if (!root) {
            // We are at the start .. just record the value
            root = std::move(value);
        } else if (currentArray != 0) {
            currentArray->push_back(std::move(value));
        } else if (currentObject != 0) {
            currentObject->insert(std::make_pair(lastPropertyName, std::move(value)));
            lastPropertyName = "";
        } else {
            // So we already have a root object .. json didn't give us any containers .. but now we're getting another value
            // We'll solve this by pretending that the top json object was a list
            PJSONValue oldRoot = std::move(root);
            startArray();
            storeValue(std::move(oldRoot));
        }
    }
public:
    JSONValueMapper() : root(), stack(), currentObject(), currentArray(), lastPropertyName() {}
    JSONValue* getValue() { return root.get(); }
    template <typename T>
    void foundSimpleValue(T value) {
        PJSONValue newVal = newJSONNumber(value);
        storeValue(std::move(newVal));
    }
    void foundSimpleValue(std::string&& value) {
        PJSONValue newVal = newJSONString(std::move(value));
        storeValue(std::move(newVal));
    }
    void foundNull() { storeValue(PJSONValue(new JSONNull())); }
    template <class T>
    T* startArrayOrObject() {
        T* result = new T();
        PJSONValue newVal = PJSONValue(result);
        stack.push_back(result);
        storeValue(std::move(newVal));
        return result;
    }
    void startArray() {
        currentArray = startArrayOrObject<JSONArray>();
    }
    void endArray() {
        stack.pop_back();
        currentArray = dynamic_cast<JSONArray*>(stack.back());
        currentObject = 0;
        if (currentArray == 0) {
            throw InvalidJSON("We're ending an array in the json, but we don't have an array on the stack");
        }
    }
    void startObj() {
        currentObject = startArrayOrObject<JSONObject>();
    }
    void endObj() {
        stack.pop_back();
        currentObject = dynamic_cast<JSONObject*>(stack.back());
        currentArray = 0;
        if (currentObject == 0) {
            throw InvalidJSON("We're ending an object in the json, but we don't have an object on the stack");
        }
    }
    void propertyName(std::string&& name) { lastPropertyName = std::move(name); }
};

}
}
