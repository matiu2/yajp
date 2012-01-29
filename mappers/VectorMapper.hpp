#ifndef YAJP_MAPPER_VECTORMAPPER_HPP
#define YAJP_MAPPER_VECTORMAPPER_HPP

#include <vector>

namespace yajp {
namespace mapper {

/** Maps a vector of simple types **/
template <class T>
class VectorMapper {
private:
    std::vector<T> _data;
public:
    VectorMapper();
    std::vector<T> result() {
        return std::move(_data);
    }
    template <class T2>
    void foundSimpleValue(const T2& value) { throw std::logic_error("Wasn't expecting this type of data"); }
    template <>
    void foundSimpleValue<T>(const T& value) { data->append(value); }
    void foundNull() { throw std::logic_error("Wasn't expecting Null"); }
    void startArray() { throw std::logic_error("Wasn't expecting a nested list"); }
    void endArray() { } // We're done now no more list
    void startObj() { throw std::logic_error("Wasn't expecting a nested object"); }
    void endObj() { throw std::logic_error("Wasn't expecting a nested object end"); }
    void propertyName(string&&) { throw std::logic_error("Wasn't expecting a property label"); }
};

} // namespace mapper
} // namespace yajp

#endif // YAJP_MAPPER_VECTORMAPPER_HPP
