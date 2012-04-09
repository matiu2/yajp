/* Ragel file for parsing json.

** History **

Originally I tried to do it all in one go. However, that's not possible as JSON is recursive.

I then tried to do it as 'in one go' as possible, thinking to make it the fastest json parser available, but that's not very flexible to program on top of.

** Summary **

So now it I've separated it out into 4 state machines for the 4 hard JSON types of Array, Dictionary, Number and String. The static ones can be read with straight loops; 'null', 'true', 'false'.


*/

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
    # Find the start of the next value
    machine start;
    _ = space**; # Stuff to ignore
    action returnLiteral { return (JSONType)*p; } 
    action returnBoolean { fhold; return JSONType::boolean; } # 'fhold' is to make sure 'frue' is not considered to be 'true'
    action returnNumber { fhold; return JSONType::number; } # Don't eat the first letter when finding a number
    start_array = '[';
    start_object = '{';
    start_boolean = [tf];
    start_number = [\-0-9.];
    start_string = '"';
    start_null = 'n';
    start_value = _.(start_array@returnLiteral|start_object@returnLiteral|start_boolean@returnBoolean|start_null@returnLiteral|start_number@returnNumber|start_string@returnLiteral);
    main := start_value;

    # Easy types first

    # Boolean machine
    #machine boolean;
    #action gotTrue { return true; }
    #action gotFalse { return false; }
    #true = "true"@gotTrue;
    #false = "false"@gotFalse;
    #main := (true|false);

    #machine null;
    #main := "ull";

    # Harder types

    # Array machine
    machine array;
    _ = space**; # Stuff to ignore
    action haveMore { return true; }
    action noMore { return false; }
    separator = _.','._;
    end = _.']';
    main := separator@haveMore|end@noMore;

    # For Reading an attribute of an object
    machine before_attribute;
    _ = space**; # Stuff to ignore
    main := _.'"';

    # For Reading an attribute of an object
    machine after_attribute;
    _ = space**; # Stuff to ignore
    main := _.':';

    # For checking if we have more object to read
    machine object;
    _ = space**; # Stuff to ignore
    action haveMore { return true; }
    action noMore { return false; }
    value_separator = _.','._;
    end = _.'}';
    main := _.(value_separator@haveMore|end@noMore);

    # string machine
    machine string;
    include string "string.rl";
    main := string;

    # Number machine
    machine number;
    include "number.rl";
    main := number;

}%%

const int STACK_JUMP_SIZE=16384; // How much memory in 'ints' to get each time the yajp stack needs an update

class JSONParser {
public:
    enum JSONType { null='n', boolean='t', array='[', object='{', number='0', string='"' };
private:
    // Ragel vars
    const char *p;
    const char *pe;
    const char *eof;
    
    void readAttributeStart() {
        %%{
            machine before_attribute;
            write data;
            write init;
            write exec;
        }%%
    }

    void readAttributeEnd() {
        %%{
            machine after_attribute;
            write data;
            write init;
            write exec;
        }%%
    }

    /**
    * Tests a null terminated C string. If we reach the null char .. just returns.
    * If something doesn't match, it throws the JSONParserError.
    *
    * @param test A null terminated C string to compare 'p' against.
    */
    void checkStaticString(const char* test) {
        while (*test)
            if ((*test++) != (*p++))
                throw JSONParserError(*this);
    }

public:
    /// @param json - KEEP THIS STRING ALIVE .. WE DONT COPY IT .. WE USE IT
    JSONParser(const std::string& json) : p(json.c_str()), pe(p+json.length()), eof(pe) {}
    JSONParser(JSONParser&& original) : p(original.p), pe(original.pe), eof(original.eof) {}

    JSONType getNextType() {
        int cs; // Current state
        %%{
            machine start;
            write data;
            write init;
            write exec;
        }%%
    }

    void readNull() { checkStaticString("ull"); }

    bool readBoolean() {
        switch (*p++) {
            case 't':
                checkStaticString("rue");
                break;
            case 'f':
                checkStaticString("alse");
                break;
            default:
                throw JSONParserError(*this);
        }
    }

    /**
    * @brief Reads in a number from the json stream
    *
    * @tparam T the type of number we expect to find, defaults to double
    *
    * @return The value of the number we read
    */
    template <typename T=double>
    T readNumber() {
        int cs; // Current state
        bool intIsNeg=false; // true if the int part is negative
        bool expIsNeg=false; // true if the exponent part is negative
        unsigned long long intPart=0; // The integer part of the number
        int expPart1=0; // The inferred exponent part gotten from counting the decimal digits
        int expPart2=0; // The explicit exponent part from the number itself, added to the inferred exponent part
        %%{
            machine string;
            write data;
            write init;
            write exec;
        }%%
    }

    std::string readString() {
        int cs; // Current state
        unsigned long uniChar = 0;
        std::string output;
        %%{
            machine string;
            write data;
            write init;
            write exec;
        }%%
        return output;
    }

    bool doIHaveMoreArray() {
        int cs; // Current state
        %%{
            machine array;
            write data;
            write init;
            write exec;
        }%%
    }

    /**
    * @brief While reading an object .. get the next attribute name
    */
    std::string getNextAttribute {
        int cs; // Current state
        readAttributeStart();
        std::string output = readString();
        readAttributeEnd();
        return output;
    }

    /**
    * @brief While reading an object .. returns true if there is more to read. Also moves us forward in the parsing.
    *
    * @return true if there is more Object to read
    */
    bool doIHaveMoreObject() {
        int cs; // Current state
        %%{
            machine object;
            write data;
            write init;
            write exec;
        }%%
    }

};


} // namespace yajp

#endif // YAJP_JSON_HPP
