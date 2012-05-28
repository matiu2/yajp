/* Ragel file for parsing json.

** History **

Originally I tried to do it all in one go. However, that's not possible as JSON is recursive.

I then tried to do it as 'in one go' as possible, thinking to make it the fastest json parser available, but that's not very flexible to program on top of.

** Summary **

I've turned a lot of the ragel state machines into hard coded switch based state machines. The only two left that are ragel 'string' and 'number'.

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
    # Harder types

    # string machine
    machine string;
    include string "string.rl";
    main := string;

    # Number machine
    machine number;
    include "number.rl";
    main := number;

}%%

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
                    case number:
                        --p;
                        return;
                    case ERROR:
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
        while ((p < pe) && (p < eof)) {
            switch( (*p++) ) {
                case 9: case 10: case 13:
                case ' ':
                    continue;
                case '"': return string;
                case '-': case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9': {
                    --p;
                    return number;
                }
                case '[': return array;
                case 'f': return boolean;
                case 'n': return null;
                case 't': return boolean;
                case '{': return object;
                default:
                    break;
            }
        }
        if ((p >= pe) || (p >= eof)) {
            returnError = true;
        }
        if (returnError)
            return p < pe ? ERROR : HIT_END;
        else {
            handleError("Couldn't Identify next JSON Type");
            return getNextType(returnError);
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
                return true;
            case 'f':
                checkStaticString("alse");
                return false;
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
        bool intIsNeg=false; // true if the int part is negative
        bool expIsNeg=false; // true if the exponent part is negative
        unsigned long long intPart=0; // The integer part of the number
        int expPart1=0; // The inferred exponent part gotten from counting the decimal digits
        int expPart2=0; // The explicit exponent part from the number itself, added to the inferred exponent part
        %%machine number;
        int startState = 
            %%write start;
        ;
        int errState =
            %%write error;
        ;
        int cs = startState; // Current state
        %%{
            write exec;
        }%%
        if (cs == errState)
            handleError("Couldn't read a number");
        return 0;
    }

    std::string readString() {
        %%machine string;
        int startState = 
            %%write start;
        ;
        int errState =
            %%write error;
        ;
        int cs = startState; // Current state
        unsigned long uniChar = 0;
        std::string output;
        %%{
            #write data nofinal noerror;
            #write init;
            write exec;
        }%%
        if (cs == errState)
            handleError("Couldn't read a string");
        return output;
    }

    /**
    * @brief checks if we have more array items to read
    *
    * @return true if there are more array to read; false if we've hit the end; throw's for unexpected hit
    */
    bool doIHaveMoreArray() {
        return doIHaveMore<']'>();
    }

    /**
    * @brief While reading an object .. get the next attribute name
    */
    std::string getNextAttribute() {
        readAttributeStart();
        std::string output = readString();
        readAttributeEnd();
        return output;
    }

    /**
    * Reads through whitespace, return true if it hits @a separator first, false if it hits @a end first. 
    * Throws a 'JSONParser' error if it hits any other non-whitespace character
    *
    * @tparam end The character that means we hit the end, no more to come
    * @tparam separator The character that means we have more to come
    *
    * @return returns true if we can expect more input, true if we just hit the end
    */
    template <char end, char separator=','>
    bool doIHaveMore() {
        while ((p < pe) && (p < eof)) {
            switch (*p++) {
            case 9:
            case 10:
            case 13:
            case ' ':
                continue;
            case separator:
                return true;
            case end:
                return false;
            default:
                handleError(std::string("Expected a '") + separator + "' or a '" + end + "' but got:");
            }
        }
        handleError(std::string("Expected a '") + separator + "' or a '" + end + "' but hit the end of the input");
    }

    /**
    * @brief While reading an object .. returns true if there is more to read. Also moves us forward in the parsing.
    *
    * @return true if there is more Object to read
    */
    bool doIHaveMoreObject() {
        return doIHaveMore<'}'>();
    }

};

} // namespace yajp

#endif // YAJP_JSON_HPP
