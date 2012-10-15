#ifndef ANYMAPPER_HPP
#define ANYMAPPER_HPP

#include "../parser/json.hpp"
namespace yajp {
namespace mappers {

boost::any json2any(JSONParser& parser);

} // namespace yajp
} // namespace mappers

#endif // ANYMAPPER_HPP
