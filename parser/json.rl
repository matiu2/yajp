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
        mapper.startArray();
        fcall array;
    }
    action endArray {
        mapper.endArray();
        fret;
    }
    action startObject {
        // Called when we hit the start of an object '{' starts recursing to get the inside parts of an object
        // The Object is recursive, as it can hold further objects, so we need to employ our fake stack and use fcall
        mapper.startObj();
        fcall object;
    }
    action endObject {
        mapper.endObj();
        fret;
    }
    action propertyName {
        // Called when we found a new property for a value
        mapper.propertyName(std::move(currentString));
        currentString.clear();
    }
    prepush {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    }
    postpop {
        if (top == 0) {
            free(stack);
            stack = 0;
        } else if ((top+1) % STACK_JUMP_SIZE == 0) {
                stack = (int*)realloc(stack, (top+1)*sizeof(int));
        }
    }

    # General JSON rules
    true = "true"@foundTrue;
    false = "false"@foundFalse;
    null_ = "null";
    _ = space**; # Stuff to ignore
    start_array = '['>parseArray;
    start_object = '{'>startObject;
    value = start_array|start_object|string@stringDone|number|true|false|null_;
    main := _.value._;

    empty_array = _.']';
    array := (empty_array|(_.(value._.','._)**.value._.']'))@endArray;

    empty_object = _.'}';
    object := (empty_object|((_.string@propertyName._.':'._.value._.','._)**.(_.string@propertyName._.':'._.value._).'}'._))@endObject;
}%%

const int STACK_JUMP_SIZE=16384; // How much memory in 'ints' to get each time the yajp stack needs an update

class JSONParser {
private:

    // All the bits it needs
    %%write data;

    // Ragel vars
    int cs; // Current state
    const char *p;
    const char *pe;
    const char *eof;

public:
    /// @param json - KEEP THIS STRING ALIVE .. WE DONT COPY IT .. WE USE IT
    JSONParser(const std::string& json) : p(json.c_str()), pe(p+json.length()), eof(pe) {}
    JSONParser(JSONParser&& original) : p(original.p), pe(original.pe), eof(original.eof) {}

    /**
    * @brief Parses a single JSON value. 
    *
    * @tparam Target The real type that we're parsing to
    * @param mapper A reference to the mapper object that maps the json callbacks to an actual c++ object
    */
    template <class Mapper>
    void parseJSONValue(Mapper& mapper) {
        int* stack = 0; // stack for ragel's fcall
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
