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
#include <stdexcept>

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
    action handleError { hitError = true; }
    start_array = '[';
    start_object = '{';
    start_boolean = [tf];
    start_number = [\-0-9.];
    start_string = '"';
    start_null = 'n';
    start_value = _.(start_array@returnLiteral|start_object@returnLiteral|start_boolean@returnBoolean|start_null@returnLiteral|start_number@returnNumber|start_string@returnLiteral);
    main := start_value$!handleError;

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
    # machine before_attribute;
    # _ = space**; # Stuff to ignore
    # main := _.'"';

    # For Reading an attribute of an object
    # machine after_attribute;
    # _ = space**; # Stuff to ignore
    # main := _.':';

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

class JSONParserError : public std::runtime_error {
private:
    std::string make_msg(const char* location, const std::string& msg) {
        return msg + " at " + location;
    }
public:
    JSONParserError(const char* location, const std::string& msg) : std::runtime_error(make_msg(location, msg)) { }
};

class JSONParser {
public:
    enum JSONType { null='n', boolean='t', array='[', object='{', number='0', string='"', HIT_END=0, ERROR='x' };
private:
    // Ragel vars
    const char *p;
    const char *pe;
    const char *eof;
    // Our vars
    bool skipOverErrors;
    
    /// Searches though whitespace for a '"' meaning the start of an attribute name
    void readAttributeStart() {
        while ((p < pe) && (p < eof)) {
            switch (*p++) {
            case 9:
            case 10:
            case 13:
            case ' ':
                continue;
            case '"':
                return;
            default:
                throw JSONParserError(p, "Couldn't find '\"' to signify the start of an attribute value");
            }
        }
        throw JSONParserError(p, "hit end while looking for '\"' to signify the start of an attribute value");
    }

    /// Searches though whitespace for a ':' meaning the change between an attribute name and an attribute value
    void readAttributeEnd() {
        while ((p < pe) && (p < eof)) {
            switch (*p++) {
            case 9:
            case 10:
            case 13:
            case ' ':
                continue;
            case ':':
                return;
            default:
                throw JSONParserError(p, "Couldn't find ':' to signify the start of an attribute value");
            }
        }
        throw JSONParserError(p, "hit end while looking for ':' to signify the start of an attribute value");
    }

    /**
    * If 'skipOverErrors' is false, throws a JSONParserError, otherwise just returns.
    * Context is automatically passed to the JSONParserError object.
    *
    * @param message The error message to show
    */
    void handleError(const std::string& message) {
        if (skipOverErrors) {
            // Skip Forward until we find a new type, then reverse one
            while (p < pe) {
                switch (getNextType(true)) {
                    JSONType::number:
                        --p;
                        return;
                    JSONType::ERROR:
                        ++p;
                        continue;
                    default:
                        return;
                }
            }
        } else
            throw JSONParserError(p, message);
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
                handleError(std::string("Static String '") + test + "' doesn't match");
    }


public:
    /// @param json - KEEP THIS STRING ALIVE .. WE DONT COPY IT .. WE USE IT
    JSONParser(const std::string& json, bool skipOverErrors=false) :
        p(json.c_str()), pe(p+json.length()), eof(pe), skipOverErrors(skipOverErrors) {}
    JSONParser(JSONParser&& original, bool skipOverErrors=false) :
        p(original.p), pe(original.pe), eof(original.eof), skipOverErrors(skipOverErrors) {}

    /**
    * Eats whitespace, then tells you the next type found in the JSON stream.
    * It eats the first letter for all types except number.
    *
    * @param returnError defaults to False .. true is used internally by 'handleError'
    *    returnError - skipOverErrors - behaviour on syntax error - on end of stream
    *      true      -   true         - returns ERROR             - returns HIT_END
    *      true      -   false        - returns ERROR             - returns HIT_END
    *      false     -   true         - skips forward until next type and returns that - returns HIT_END
    *      false     -   false        - throws JSONParserError    - throws JSONParserError
    *
    * @return the 'JSONType' found,
    */
    JSONType getNextType(bool returnError=false) {
        int cs; // Current state
        bool hitError = false; // Set to true by state machine if we hit an error.
        %%{
            machine start;
            write data;
            write init;
            write exec;
        }%%
        if (hitError) {
            if (returnError)
                return p < pe ? ERROR : HIT_END;
            else
                handleError("Couldn't Identify next JSON Type");
        }
    }

    void readNull() { checkStaticString("ull"); }

    /**
    * Read a boolean value, either 'true' or 'false'. Assume's you've already found it with 'getNextType'
    *
    * @return The value of the boolean read.
    */
    bool readBoolean() {
        switch (*p++) {
            case 't':
                checkStaticString("rue");
                true;
            case 'f':
                checkStaticString("alse");
                false;
            default:
                handleError("Couldn't read 'true' nor 'false'");
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
            machine number;
            write data nofinal noerror;
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
            write data nofinal noerror;
            write init;
            write exec;
        }%%
        return output;
    }

    bool doIHaveMoreArray() {
        int cs; // Current state
        %%{
            machine array;
            write data nofinal noerror;
            write init;
            write exec;
        }%%
    }

    /**
    * @brief While reading an object .. get the next attribute name
    */
    std::string getNextAttribute() {
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
            write data  nofinal noerror;
            write init;
            write exec;
        }%%
    }

};

} // namespace yajp

#endif // YAJP_JSON_HPP
