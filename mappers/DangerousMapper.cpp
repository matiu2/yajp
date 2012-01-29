#include "DangerousMapper.hpp"
#include <map>
#include <string>
#include <stdexcept>

namespace yajp {
namespace mapper {

struct DangerousMapper::impl {
    std::map<std::string, void*> attributeMap;
};

DangerousMapper::DangerousMapper() : m(new impl()) { }

void DangerousMapper::doMapProperty(const std::string& attributeName, void* value) { m->attributeMap.insert(std::make_pair(attributeName, value); }

template <typename T>
void DangerousMapper::applyProperty(const std::string& attributeName, T value) {
    void* target = m->attributeMap.get(attributeName);
    if (target == 0)
        throw std::logic_error("Could not apply attribute value: '" + attributeName + "'");
    reinterpret_cast<T&>(*target) = value; // DANGEROUS CAST
}

} // namespace mapper
} // namespace yajp
