/// Allows you to parse json into yajp::JSONType classes
#ifndef JSONVALUEMAPPER_HPP
#define JSONVALUEMAPPER_HPP

#include "../json_type.hpp"
#include "../parser/json.hpp"
#include <memory>
#include <stdexcept>

namespace yajp {

pJSONType parseToClasses(JSONParser& parser) {
    switch (parser.getNextType()) {
        case JSONParser::null:
            return pJSONType(new JSONNull());
        case JSONParser::boolean:
            return pJSONType(new JSONBool(parser.readBoolean()));
        case JSONParser::array: {
            std::unique_ptr<JSONArray> array(new JSONArray);
            while (parser.doIHaveMoreArray())
                array->addChild(parseToClasses(parser));
            return pJSONType(std::move(array));
        }
        case JSONParser::object: {
            std::unique_ptr<JSONObject> object(new JSONObject());
            while (parser.doIHaveMoreObject()) {
                std::string attrName = parser.readNextAttribute();
                object->setChild(attrName, std::move(parseToClasses(parser)));
            }
        }
        case JSONParser::number:
            return pJSONType(new JSONNumber<double>(parser.readNumber()));
        case JSONParser::string:
            return pJSONType(new JSONString(parser.readString()));
        case JSONParser::HIT_END:
        case JSONParser::ERROR:
        default:
            // Should never get here
            throw std::logic_error("Unexpected error while parsing JSON");
    }
}

} // namespace yajp

#endif // JSONVALUEMAPPER_HPP

