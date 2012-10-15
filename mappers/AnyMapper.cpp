/**
  Takes a json parser and returns a bunch of 'boost::any' objects.
**/

#include "AnyMapper.hpp"
#include <boost/any.hpp>

namespace yajp {
namespace mappers {

class AnyMapper {
public:
    AnyMapper(JSONParser& parser);
};

boost::any json2any(JSONParser& parser) {

}

} // namespace yajp
} // namespace mappers
