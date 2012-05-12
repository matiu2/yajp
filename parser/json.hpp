
#line 1 "/Users/matiu/projects/yajp/parser/json.rl"
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


#line 40 "/Users/matiu/projects/yajp/parser/json.rl"


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
        handleError("Couldn't Identify next JSON Type");
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
        
#line 214 "/Users/matiu/projects/yajp/parser/json.rl"
        int startState = 
            
#line 211 "/Users/matiu/projects/yajp/parser/json.hpp"
1
#line 216 "/Users/matiu/projects/yajp/parser/json.rl"
        ;
        int errState =
            
#line 217 "/Users/matiu/projects/yajp/parser/json.hpp"
0
#line 219 "/Users/matiu/projects/yajp/parser/json.rl"
        ;
        int cs = startState; // Current state
        
#line 223 "/Users/matiu/projects/yajp/parser/json.hpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 45 )
		goto tr0;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 7 "/Users/matiu/projects/yajp/parser/number.rl"
	{
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 251 "/Users/matiu/projects/yajp/parser/json.hpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 13 "/Users/matiu/projects/yajp/parser/number.rl"
	{
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 269 "/Users/matiu/projects/yajp/parser/json.hpp"
	switch( (*p) ) {
		case 46: goto st3;
		case 69: goto st4;
		case 101: goto st4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
tr3:
#line 20 "/Users/matiu/projects/yajp/parser/number.rl"
	{
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 300 "/Users/matiu/projects/yajp/parser/json.hpp"
	switch( (*p) ) {
		case 69: goto st4;
		case 101: goto st4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 43: goto tr4;
		case 45: goto tr4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr4:
#line 28 "/Users/matiu/projects/yajp/parser/number.rl"
	{
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 333 "/Users/matiu/projects/yajp/parser/json.hpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr5:
#line 35 "/Users/matiu/projects/yajp/parser/number.rl"
	{
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
    }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 351 "/Users/matiu/projects/yajp/parser/json.hpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 6: 
	case 7: 
	case 8: 
#line 42 "/Users/matiu/projects/yajp/parser/number.rl"
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        T result = intPart;
        if (expPart < 0) {
            while (expPart++ < 0)
                result *= 0.1;
            return intIsNeg ? -result : result;
        } else {
            while (expPart-- > 0)
                result *= 10;
            return intIsNeg ? -result : result;
        }
    }
	break;
#line 389 "/Users/matiu/projects/yajp/parser/json.hpp"
	}
	}

	_out: {}
	}

#line 225 "/Users/matiu/projects/yajp/parser/json.rl"

        if (cs == errState)
            handleError("Couldn't read a number");
    }

    std::string readString() {
        
#line 232 "/Users/matiu/projects/yajp/parser/json.rl"
        int startState = 
            
#line 407 "/Users/matiu/projects/yajp/parser/json.hpp"
1
#line 234 "/Users/matiu/projects/yajp/parser/json.rl"
        ;
        int errState =
            
#line 413 "/Users/matiu/projects/yajp/parser/json.hpp"
0
#line 237 "/Users/matiu/projects/yajp/parser/json.rl"
        ;
        int cs = startState; // Current state
        unsigned long uniChar = 0;
        std::string output;
        
#line 421 "/Users/matiu/projects/yajp/parser/json.hpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 9 "/Users/matiu/projects/yajp/parser/string.rl"
	{ output += *p; }
	goto st1;
tr3:
#line 4 "/Users/matiu/projects/yajp/parser/string.rl"
	{ output += '\b'; }
	goto st1;
tr4:
#line 5 "/Users/matiu/projects/yajp/parser/string.rl"
	{ output += '\f'; }
	goto st1;
tr5:
#line 6 "/Users/matiu/projects/yajp/parser/string.rl"
	{ output += '\n'; }
	goto st1;
tr6:
#line 7 "/Users/matiu/projects/yajp/parser/string.rl"
	{ output += '\r'; }
	goto st1;
tr7:
#line 8 "/Users/matiu/projects/yajp/parser/string.rl"
	{ output += '\t'; }
	goto st1;
tr11:
#line 21 "/Users/matiu/projects/yajp/parser/string.rl"
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            output += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            output += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            output += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            output += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            output += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            output += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
#line 9 "/Users/matiu/projects/yajp/parser/string.rl"
	{ output += *p; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 478 "/Users/matiu/projects/yajp/parser/json.hpp"
	switch( (*p) ) {
		case 34: goto st5;
		case 92: goto st2;
	}
	goto tr0;
tr12:
#line 21 "/Users/matiu/projects/yajp/parser/string.rl"
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            output += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            output += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            output += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            output += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            output += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            output += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 509 "/Users/matiu/projects/yajp/parser/json.hpp"
	goto st0;
st0:
cs = 0;
	goto _out;
tr13:
#line 21 "/Users/matiu/projects/yajp/parser/string.rl"
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            output += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            output += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            output += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            output += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            output += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            output += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 539 "/Users/matiu/projects/yajp/parser/json.hpp"
	switch( (*p) ) {
		case 98: goto tr3;
		case 102: goto tr4;
		case 110: goto tr5;
		case 114: goto tr6;
		case 116: goto tr7;
		case 117: goto st3;
	}
	goto tr0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr9;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr9;
	} else
		goto tr9;
	goto st0;
tr9:
#line 11 "/Users/matiu/projects/yajp/parser/string.rl"
	{
        uniChar <<= 4;
        char ch = *p;
        if (ch >= 'a') 
            uniChar += ch - 'a' + 0x0A;
        else if (ch >= 'A')
            uniChar += ch - 'A' + 0x0A;
        else
            uniChar += ch - '0';
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 579 "/Users/matiu/projects/yajp/parser/json.hpp"
	switch( (*p) ) {
		case 34: goto tr12;
		case 92: goto tr13;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr9;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr9;
	} else
		goto tr9;
	goto tr11;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 242 "/Users/matiu/projects/yajp/parser/json.rl"
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
