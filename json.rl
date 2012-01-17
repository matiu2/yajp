/* Ragel file for parsing json */

#pragma once
#ifndef YAJP_JSON_HPP
#define YAJP_JSON_HPP

#include <string>
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {

%%{
    machine json;
    include "string.rl";
    include "number.rl";

    # General JSON Actions
    action foundTrue { mapper.foundSimpleValue(true); }
    action foundFalse { mapper.foundSimpleValue(false); }
    action foundNull { mapper.foundNull(); }
    action parseArray {
        // Called when we hit the start of an array
        // The Array is recursive, as it can hold further arrays, so we need to employ our fake stack and use fcall
        fhold;
        mapper.startArray();
        fcall array;
        mapper.endArray();
    }
    action parseObject {
        // Called when we hit the start of an object '{' starts recursing to get the inside parts of an object
        // The Object is recursive, as it can hold further objects, so we need to employ our fake stack and use fcall
        fhold;
        mapper.startObj();
        fcall object;
        mapper.endObj();
    }
    action propertyName {
        // Called when we found a new property for a value
        mapper.propertyName(std::move(currentString));
    }

    # General JSON rules
    true = "true"@foundTrue;
    false = "false"@foundFalse;
    null_ = "null";
    _ = space**; # Stuff to ignore
    start_array = '['>parseArray;
    start_object = '{'>parseObject;
    value = start_array|start_object|string@stringDone|number|true|false|null_;
    main := _.value._;

    empty_array = '['._.']';
    array := empty_array|('['._.(value._.','._)**.value._.']');

    empty_object = '{'._.'}';
    object := empty_object|('{'.(_.string@propertyName._.':'._.value._.','._)**.(_.string@propertyName._.':'._.value._).'}'._);
}%%

class JSONParser {
private:
    %%write data;

    // Ragel vars
    int cs; // Current state
    const char *p = &json.c_str()[0];
    const char *pe = p + json.length();
    const char *eof = pe;

    class JSONParser(const std::string& json) : p(json.c_str()[0]), pe(json.length()), eof(pe) {}
    class JSONParser(JSONParser&& original) : p(original.p), pe(original.pe), eof(original.eof) {}

    /**
    * @brief Parses a single JSON value. 
    *
    * @tparam Target The real type that we're parsing to
    * @param target A reference to the C++ value that we're filling in
    */
    template <class Target>
    void parseJSONValue(Target& target) {
        Mapper<Target> mapper;
        std::vector<int> stack; // fake stack for ragel's fcall
        int top = 0;
        // int machine action vars
        bool intIsNeg=false; // true if the int part is negative
        bool expIsNeg=false; // true if the exponent part is negative
        unsigned long long intPart=0; // The integer part of the number
        long expPart1=0; // The inferred exponent part gotten from counting the decimal digits
        long expPart2=0; // The explicit exponent part from the number itself, added to the inferred exponent part
        // string machine action vars
        unsigned long uniChar = 0;
        std::string currentString;
        // Initialization of state machine
        %%write init;
        // Execution of state machine
        %%write exec;
    }
};


} // namespace yajp

#endif // YAJP_JSON_HPP
