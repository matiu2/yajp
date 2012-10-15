/**
 * A hirarchy of classes that represent the basic types enabled in JSON.
 *
 * This is probably not the fastest option for reading in json types,
 * due to the use of the virtual method table. However it should be
 * quite straight forward and easy to use.
 *
 * The mappers/JSONValueMapper.hpp file provides method's to just read JSON
 * straight in to JSONType instances without needing to know what order the json is in;
 * you can later inspect the hierarchy of instances outputted.
**/

#include <cstddef>
#include <stdexcept>
#include <typeinfo>

namespace yajp {

class JSONContainer;
class JSONBool;
class JSONNull;
class JSONString;
class JSONArray;
class JSONObject;

struct JSONError : public std::runtime_error {
    JSONError(const std::string& msg) : std::runtime_error(msg) {}
};

class JSONType {
public:
    enum JSONTypeEnum {
        Unassigned,
        String,
        Number,
        Boolean,
        Null,
        Array,
        Object
    };
public:
    virtual JSONTypeEnum getType() const { return Unassigned; }
    // Casting operators
    operator std::string() {
        JSONString* asString = dynamic_cast<JSONString*>(this);
        if (!asString)
            throw std::bad_cast();
        return asString->value();
    }
    template <typename T>
    operator T() {
        JSONNumber<T>* asNum = dynamic_cast<JSONNumber<T>*>(this);
        if (!asNum) {
            // Try as a double and do a straight int cast
            JSONNumber<double>* asDouble = dynamic_cast<JSONNumber<double>*>(this);
            if (!asNum)
                throw std::bad_cast();
            return (T)asDouble->value();
        }
        return asNum->value();
    }
    template <typename T>
    operator std::vector<T>() {
        JSONArray* asArray = dynamic_cast<JSONArray*>(this);
        if (!asArray)
            throw std::bad_cast();
        std::vector<T> result(asArray->size());
        auto i = relult.begin();
        for (pJSONValue value : asArray()) {
            *(i++) = *value;
        }
        return result;
    }
};

typedef std::unique_ptr<JSONType> pJSONType;

class JSONString : public JSONType {
private:
    std::string _value;
public:
    JSONString(const std::string& value) : _value() {}
    const std::string& value() const { return _value; }
    virtual JSONTypeEnum getType() const { return String; }
    virtual opertor std::string() override { return value(); }
};

class JSONBool : public JSONType {
private:
    bool _value;
public:
    JSONBool(bool value) : JSONType(), _value() {}
    bool value() const { return _value; }
    virtual JSONTypeEnum getType() const { return Boolean; }
};

class JSONNull : public JSONType {
public:
    JSONNull() : JSONType() {}
    virtual JSONTypeEnum getType() const { return Null; }
};

/// The base class for all container types
class JSONContainer : public JSONType {
public:
    JSONContainer() : JSONType() {}
};
typedef std::unique_ptr<JSONContainer> pJSONContainer;

/// The template class for the two kinds of containers, array and object
template <class HolderType, typename IndexType>
class SpecificContainer : public JSONContainer{
protected:
    HolderType _children;
public:
    SpecificContainer() : JSONContainer(), HolderType() {}
    SpecificContainer(std::initializer_list<typename HolderType::value_type> list) : JSONContainer(), HolderType(list) {}
    SpecificContainer(std::initializer_list<typename HolderType::value_type>&& list) : JSONContainer(), HolderType(std::move(list)) {}
    SpecificContainer(SpecificContainer<HolderType, IndexType>&& other) : JSONContainer(), HolderType(std::move(other._children)) {}
    JSONType& getChild(IndexType index) { return *_children[index]; }
    const JSONType& getChild(IndexType index) const { return *_children[index]; }
    void setChild(IndexType index, pJSONType value) { _children[index] = std::move(value); }
    size_t size() const  { return _children.size(); }
};

typedef SpecificContainer<std::vector<pJSONType>, std::vector<pJSONType>::size_type> JSONArrayParent;

class JSONArray : public JSONArrayParent {
friend class JSONObject;
public:
    JSONArray() : JSONArrayParent() {}
    JSONArray(std::initializer_list<pJSONType> children) : JSONArrayParent(children) {}
    virtual JSONTypeEnum getType() const { return Array; }
    void addChild(pJSONType value) {
        _children.push_back(std::move(value));
    }
};
typedef std::unique_ptr<JSONArray> pJSONArray;


/** This is the base class of all the number template classes.
  * It enables the user to just check, "is it a number?"
  **/
class JSONNumberBase : public JSONType {
public:
    JSONNumberBase() : JSONType() {}
    virtual JSONTypeEnum getType() const { return Number; }
};
typedef std::unique_ptr<JSONNumberBase> pJSONNumberBase;

template <typename NumberType=double>
struct JSONNumber : JSONNumberBase  {
private:
    NumberType _value;
public:
    JSONNumber(const NumberType& value) : JSONNumberBase(), _value(value) {}
    NumberType value() const { return _value; }
};
typedef std::unique_ptr<JSONNumber<double>> pJSONNumber;

typedef std::map<std::string, pJSONType> StringJSONMap;
typedef SpecificContainer<StringJSONMap, const std::string&> JSONObjectParent;

class JSONObject : public JSONObjectParent {
public:
    JSONObject() : JSONObjectParent() {}
    JSONObject(std::initializer_list<StringJSONMap::value_type> children) : JSONObjectParent(children) {}
    JSONObject(JSONObject&& other) : JSONObjectParent(std::move(other)) {}
    const std::string& getString(const std::string key) const {
        StringJSONMap::const_iterator result(_children.find(key));
        if (result == _children.end())
            throw JSONError(std::string("Couldn't find a string with key: ") + key);
        const JSONString* asJSONString(dynamic_cast<JSONString*>(result->second.get()));
        if (asJSONString)
            return asJSONString->value();
        else
            throw JSONError(std::string("Couldn't find a string with key: ") + key);
    }
    template<typename NumberType=double>
    NumberType getNumber(const std::string& key) const {
        StringJSONMap::const_iterator result(_children.find(key));
        if (result == _children.end())
            throw JSONError(std::string("Couldn't find a number with key: ") + key);
        JSONNumber<NumberType>* asNumber(dynamic_cast<JSONNumber<NumberType>>(result->second.get()));
        if (asNumber)
            return asNumber->value();
        else {
            JSONNumber<double>* asDouble(dynamic_cast<JSONNumber<double>>(result->second.get()));
            if (asDouble)
                return (NumberType) asDouble->value();
        }
        throw JSONError(std::string("Couldn't find a number with key: ") + key);
    }
    std::vector<std::string> getVector(const std::string& key) {
        StringJSONMap::const_iterator data(_children.find(key));
        if (data == _children.end())
            throw JSONError(std::string("Couldn't find an array with key: ") + key);
        JSONArray* asJSONArray(dynamic_cast<JSONArray*>(data->second.get()));
        if (!asJSONArray)
            throw JSONError(std::string("Couldn't find an array under key: ") + key);
        std::vector<std::string> result(asJSONArray->size());
        auto i=result.begin();
        for (const auto& jsonVal : asJSONArray->_children) {
            JSONString* asJSONString(dynamic_cast<JSONString*>(jsonVal.get()));
            if (!asJSONString)
                throw JSONError(std::string("Not all members of array were strings: ") + key);
            *i = asJSONString->value();
        }
        return result;
    }
};
typedef std::unique_ptr<JSONObject> pJSONObject;
} // namespace yajp
