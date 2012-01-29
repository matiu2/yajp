#ifndef YAJP_MAPPER_MAPPER_HPP
#define YAJP_MAPPER_MAPPER_HPP

#include <string>
#include <stdexcept>

namespace yajp {
namespace mappers {

/** Mapper BaseClass. Complains about everything **/
class Mapper {
public:
    template <typename T>
    void foundSimpleValue(T) {
        throw std::logic_error(std::string("Didn't expect value of type '") + typeid(T).name() + "' at this point");
    }
    void foundNull() {
        throw std::logic_error("Didn't expect null value here");
    }
    void startArray() {
        throw std::logic_error("Didn't expect the start of an array here");
    }
    void endArray() {
        throw std::logic_error("Didn't expect the end of an array here");
    }
    void startObj() {
        throw std::logic_error("Didn't expect the start of an object here");
    }
    void endObj() {
        throw std::logic_error("Didn't expect the end of an object here");
    }
    void propertyName(std::string&&) {
        throw std::logic_error("Didn't expect a property name here");
    }
};

template <typename T>
class SimpleMapper : public Mapper {
private:
    T& result;
public:
    SimpleMapper(T& result) : result(result) {}
    template <>
    void foundSimpleValue<T>(const T& value) { result = value; }
};

template <typename T>
class SimpleMoveMapper : public Mapper {
private:
    T& result;
public:
    SimpleMoveMapper(T& result) : result(result) {}
    template <>
    void foundSimpleValue<T>(T&& value) { result = std::move(value); }
};

} // namespace mapper
} // namespace yajp

#endif // YAJP_MAPPER_MAPPER_HPP
