#ifndef YAJP_MAPPER_DANGEROUSMAPPER_HPP
#define YAJP_MAPPER_DANGEROUSMAPPER_HPP

#include <memory>

namespace yajp {
namespace mapper {

/** This is a dangerous, but hopefully fast mapper.
  * It maps callbacks from the JSON parser into real classes.
  * In order to use it, your class must provide a template method that helps map json property values to real life types.
  * For example:
  * struct Person() {
  *   std::string name;
  *   int age;
  *   vector<string> diplomas;
  *   void map2Json(DangerousMapper& mapper) {
  *       mapper.mapProperty("name", name);
  *       mapper.mapProperty("age", age);
  *       mapper.mapProperty("diplomas", diplomas);
  *   }
  **/
class DangerousMapper {
private:
    class impl;
    std::unique_ptr<impl> m;
    void doMapProperty(const std::string& attributeName, void* value);
public:
    DangerousMapper();
    template <typename T>
    void mapProperty(const std::string& attributeName, T& value) { doMapProperty(attributeName, (void*)&value;}
    extern template <typename T>
    void applyProperty(const std::string& attributeName, T value);
};

} // namespace mapper
} // namespace yajp

#endif // YAJP_MAPPER_DANGEROUSMAPPER_HPP
