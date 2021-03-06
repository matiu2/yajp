
#line 1 "/home/matthew/projects/yajp/parser/json.rl"
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
#include <functional>
#include <vector>
#include <sstream>
#include <map>

#ifdef DEBUG
#include <iostream>
#endif


namespace yajp {


#line 46 "/home/matthew/projects/yajp/parser/json.rl"


/// Allows lazy evaluation of number types
class JSONNumberInfo {
private:
    bool _intIsNeg;
    unsigned long long _intPart;
    int _expPart;
public:
    JSONNumberInfo(bool intIsNeg, unsigned long long intPart, int expPart) : _intIsNeg(intIsNeg), _intPart(intPart), _expPart(expPart) {}

    template<typename NumberType>
    NumberType value() const {
        NumberType result = _intPart;
        int expPart = _expPart;
        if (expPart < 0) {
            while (expPart++ < 0)
                result *= 0.1;
            return _intIsNeg ? -result : result;
        } else {
            while (expPart-- > 0)
                result *= 10;
            return _intIsNeg ? -result : result;
        }
    }

    operator int() { return value<int>(); }
    operator unsigned int() { return value<unsigned int>(); }
    operator long() { return value<long>(); }
    operator unsigned long() { return value<unsigned long>(); }
    operator long long() { return value<long long>(); }
    operator unsigned long long() { return value<unsigned long long>(); }
    operator float() { return value<float>(); }
    operator double() { return value<double>(); }
    operator short() { return value<short>(); }
    operator unsigned short() { return value<unsigned short>(); }
    operator char() { return value<char>(); }
    operator unsigned char() { return value<unsigned char>(); }
};

class JSONParser;

class JSONParserError : public std::runtime_error {
private:
    const JSONParser* _parser;
    const char* _location;
    std::string _msg;
    std::string make_msg(const char* location, const std::string& msg) {
        return msg + " at " + location;
    }
public:
    JSONParserError(const JSONParser* parser, const char* location, const std::string& msg) :
        std::runtime_error(make_msg(location, msg)),
        _parser(parser),
        _location(location),
        _msg(msg) { }
    const JSONParser* parser() const { return _parser; }
    const char* location() const { return _location; }
    const std::string& msg() const { return _msg; }
};

class JSONParser {
public:
    enum JSONType { null='n', boolean='t', array='[', object='{', number='0', string='"', HIT_END=0, ERROR='x' };
    typedef uint32_t UniChar; // 32 bit unicode characters max
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
                throw JSONParserError(this, p, "Couldn't find '\"' to signify the start of an attribute value");
            }
        }
        throw JSONParserError(this, p, "hit end while looking for '\"' to signify the start of an attribute value");
    }

    /**
    * If 'skipOverErrors' is false, throws a JSONParserError, otherwise just returns.
    * Context is automatically passed to the JSONParserError object.
    *
    * @param message The error message to show
    */
    void handleError(const std::string& message) {
        if (skipOverErrors) {
            // Skip Forward until we find a new type
            while (p < pe) {
                switch (getNextType(true)) {
                    case number:
                        --p; // Put the read cursor in the right position to read the number
                        return;
                    case ERROR:
                        ++p;
                        continue;
                    case HIT_END:
                        // We have to raise an error here. There's no way we can skip forward anymore
                        throw JSONParserError(this, p, std::string("Hit end: ") + message);
                    default:
                        return;
                }
            }
        } else
            throw JSONParserError(this, p, message);
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

    /**
     * Returns the number of bytes needed to encode a unicode character in utf8
     */ 
    int getNumBytes (UniChar c) {
        int result = 1;
        UniChar checker = 0x7f;
        if (c <= checker)
            return result;
        checker <<= 4;
        checker |= 0xf;
        result += 1;
        if (c <= checker)
            return result;
        while (c > checker) {
            checker <<= 5;
            checker |= 0x1f;
            ++result;
        }
        return result;
    }


public:
    /// @param json - KEEP THIS STRING ALIVE .. WE DONT COPY IT .. WE USE IT .. You can't just call it with an ("inplace string")
    JSONParser(const std::string& json, bool skipOverErrors=false) :
        p(json.c_str()), pe(p+json.length()), eof(pe), skipOverErrors(skipOverErrors) {}
    /// @param json - KEEP THIS STRING ALIVE .. WE DONT COPY IT .. WE USE IT .. You can't just call it with an ("inplace string")
    JSONParser(const char* json, const char* end, bool skipOverErrors=false) :
        p(json), pe(end), eof(pe), skipOverErrors(skipOverErrors) {}
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
            case 'r':
                checkStaticString("ue");
                return true;
            case 'a':
                checkStaticString("lse");
                return false;
            default:
                handleError("Couldn't read 'true' nor 'false'");
                // Once the code reaches here, it means that error raising is turned off
                // And we couldn't read a boolean value, so it has skipped us ahead to the start
                // of the next json value. We'll just keep trying to read the boolean until we get it
                // or we stack overflow.
                return readBoolean();
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
        bool gotAtLeastOneDigit = false;
        auto makeJSONNumber = [&expIsNeg, &expPart1, &expPart2, &intIsNeg, &intPart]() {
            long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
            return JSONNumberInfo(intIsNeg, intPart, expPart);
        };
        
#line 299 "/home/matthew/projects/yajp/parser/json.rl"
        int startState = 
            
#line 294 "/home/matthew/projects/yajp/parser/json.hpp"
1
#line 301 "/home/matthew/projects/yajp/parser/json.rl"
        ;
        int cs = startState; // Current state
        
#line 300 "/home/matthew/projects/yajp/parser/json.hpp"
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
#line 7 "/home/matthew/projects/yajp/parser/number.rl"
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
#line 328 "/home/matthew/projects/yajp/parser/json.hpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 13 "/home/matthew/projects/yajp/parser/number.rl"
	{
        gotAtLeastOneDigit = true;
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
#line 347 "/home/matthew/projects/yajp/parser/json.hpp"
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
#line 21 "/home/matthew/projects/yajp/parser/number.rl"
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
#line 378 "/home/matthew/projects/yajp/parser/json.hpp"
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
#line 29 "/home/matthew/projects/yajp/parser/number.rl"
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
#line 411 "/home/matthew/projects/yajp/parser/json.hpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr5:
#line 36 "/home/matthew/projects/yajp/parser/number.rl"
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
#line 429 "/home/matthew/projects/yajp/parser/json.hpp"
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
#line 43 "/home/matthew/projects/yajp/parser/number.rl"
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        return makeJSONNumber();
    }
	break;
#line 457 "/home/matthew/projects/yajp/parser/json.hpp"
	}
	}

	_out: {}
	}

#line 305 "/home/matthew/projects/yajp/parser/json.rl"

        // The state machine returns, so the code will only get here if it can't parse the string
        if (gotAtLeastOneDigit)
            return makeJSONNumber();
        else
            handleError("Couldn't read a number");
        return T();
    }

    std::string readString() {
        
#line 316 "/home/matthew/projects/yajp/parser/json.rl"
        int startState = 
            
#line 479 "/home/matthew/projects/yajp/parser/json.hpp"
1
#line 318 "/home/matthew/projects/yajp/parser/json.rl"
        ;
        int cs = startState; // Current state
        UniChar uniChar = 0;
        int uniCharBytes = 0;
        std::string output;
        
#line 488 "/home/matthew/projects/yajp/parser/json.hpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 9 "/home/matthew/projects/yajp/parser/string.rl"
	{ output += *p; }
	goto st1;
tr3:
#line 4 "/home/matthew/projects/yajp/parser/string.rl"
	{ output += '\b'; }
	goto st1;
tr4:
#line 5 "/home/matthew/projects/yajp/parser/string.rl"
	{ output += '\f'; }
	goto st1;
tr5:
#line 6 "/home/matthew/projects/yajp/parser/string.rl"
	{ output += '\n'; }
	goto st1;
tr6:
#line 7 "/home/matthew/projects/yajp/parser/string.rl"
	{ output += '\r'; }
	goto st1;
tr7:
#line 8 "/home/matthew/projects/yajp/parser/string.rl"
	{ output += '\t'; }
	goto st1;
tr11:
#line 26 "/home/matthew/projects/yajp/parser/string.rl"
	{
        /*
           UCS-4 range (hex.)           UTF-8 octet sequence (binary)
           0000 0000-0000 007F   0xxxxxxx
           0000 0080-0000 07FF   110xxxxx 10xxxxxx
           0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx

           0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
           0020 0000-03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
           0400 0000-7FFF FFFF   1111110x 10xxxxxx ... 10xxxxxx
        */
        int numBytes = getNumBytes(uniChar); // number of bytes needed for utf-8 encoding
        if (numBytes == 1) {
            output += uniChar;
        } else {
            std::vector<char> bytes;
            for (int i=1; i<numBytes; ++i) {
                char byte = uniChar & 0x3f; // Only encoding 6 bits right now
                byte |= 0x80; // Make sure the high bit is set
                bytes.push_back(byte);
                uniChar >>= 6;
            }
            // The last byte is special
            char mask = 0x3f >> (numBytes - 2);
            char byte = uniChar & mask;
            char top = 0xc0;
            for (int i=2; i<numBytes; ++i) {
                top >>= 1;
                top |= 0x80;
            }
            byte |= top;
            bytes.push_back(byte);
            // Output it
            for (auto i=bytes.rbegin(); i!=bytes.rend(); ++i)
                output += *i;
        }
    }
#line 9 "/home/matthew/projects/yajp/parser/string.rl"
	{ output += *p; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 564 "/home/matthew/projects/yajp/parser/json.hpp"
	switch( (*p) ) {
		case 34: goto tr1;
		case 92: goto st2;
	}
	goto tr0;
tr1:
#line 63 "/home/matthew/projects/yajp/parser/string.rl"
	{
        ++p;
        return output;
    }
	goto st5;
tr12:
#line 26 "/home/matthew/projects/yajp/parser/string.rl"
	{
        /*
           UCS-4 range (hex.)           UTF-8 octet sequence (binary)
           0000 0000-0000 007F   0xxxxxxx
           0000 0080-0000 07FF   110xxxxx 10xxxxxx
           0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx

           0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
           0020 0000-03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
           0400 0000-7FFF FFFF   1111110x 10xxxxxx ... 10xxxxxx
        */
        int numBytes = getNumBytes(uniChar); // number of bytes needed for utf-8 encoding
        if (numBytes == 1) {
            output += uniChar;
        } else {
            std::vector<char> bytes;
            for (int i=1; i<numBytes; ++i) {
                char byte = uniChar & 0x3f; // Only encoding 6 bits right now
                byte |= 0x80; // Make sure the high bit is set
                bytes.push_back(byte);
                uniChar >>= 6;
            }
            // The last byte is special
            char mask = 0x3f >> (numBytes - 2);
            char byte = uniChar & mask;
            char top = 0xc0;
            for (int i=2; i<numBytes; ++i) {
                top >>= 1;
                top |= 0x80;
            }
            byte |= top;
            bytes.push_back(byte);
            // Output it
            for (auto i=bytes.rbegin(); i!=bytes.rend(); ++i)
                output += *i;
        }
    }
#line 63 "/home/matthew/projects/yajp/parser/string.rl"
	{
        ++p;
        return output;
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 626 "/home/matthew/projects/yajp/parser/json.hpp"
	goto st0;
st0:
cs = 0;
	goto _out;
tr13:
#line 26 "/home/matthew/projects/yajp/parser/string.rl"
	{
        /*
           UCS-4 range (hex.)           UTF-8 octet sequence (binary)
           0000 0000-0000 007F   0xxxxxxx
           0000 0080-0000 07FF   110xxxxx 10xxxxxx
           0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx

           0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
           0020 0000-03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
           0400 0000-7FFF FFFF   1111110x 10xxxxxx ... 10xxxxxx
        */
        int numBytes = getNumBytes(uniChar); // number of bytes needed for utf-8 encoding
        if (numBytes == 1) {
            output += uniChar;
        } else {
            std::vector<char> bytes;
            for (int i=1; i<numBytes; ++i) {
                char byte = uniChar & 0x3f; // Only encoding 6 bits right now
                byte |= 0x80; // Make sure the high bit is set
                bytes.push_back(byte);
                uniChar >>= 6;
            }
            // The last byte is special
            char mask = 0x3f >> (numBytes - 2);
            char byte = uniChar & mask;
            char top = 0xc0;
            for (int i=2; i<numBytes; ++i) {
                top >>= 1;
                top |= 0x80;
            }
            byte |= top;
            bytes.push_back(byte);
            // Output it
            for (auto i=bytes.rbegin(); i!=bytes.rend(); ++i)
                output += *i;
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 675 "/home/matthew/projects/yajp/parser/json.hpp"
	switch( (*p) ) {
		case 98: goto tr3;
		case 102: goto tr4;
		case 110: goto tr5;
		case 114: goto tr6;
		case 116: goto tr7;
		case 117: goto tr8;
	}
	goto tr0;
tr8:
#line 10 "/home/matthew/projects/yajp/parser/string.rl"
	{
        uniChar = 0;
        uniCharBytes = 0;
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 696 "/home/matthew/projects/yajp/parser/json.hpp"
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
#line 14 "/home/matthew/projects/yajp/parser/string.rl"
	{
        if (uniCharBytes++ == 4)
            throw std::logic_error("Max unicode char is 32 bits");
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
#line 725 "/home/matthew/projects/yajp/parser/json.hpp"
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

#line 327 "/home/matthew/projects/yajp/parser/json.rl"

        // The state machine returns, so the code will only get here if it can't parse the string
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
    std::string readNextAttribute() {
        readAttributeStart();
        std::string output = readString();
        return output;
    }

    /**
      * Consumes and throws away one value
      **/
    void consumeOneValue() {
        switch (getNextType()) {
            case JSONParser::null:
                readNull();
                return;
            case JSONParser::boolean:
                readBoolean();
                return;
            case JSONParser::array:
                while (doIHaveMoreArray()) {
                    consumeOneValue();
                }
                return;
            case JSONParser::object:
                while (doIHaveMoreObject()) {
                    readNextAttribute();
                    consumeOneValue();
                }
                return;
            case JSONParser::number:
                readNumber<int>();
                return;
            case JSONParser::string:
                readString();
                return;
            case JSONParser::HIT_END:
                return;
            case JSONParser::ERROR:
                return; // Code should never hit here
        };
    }

    /**
    * Reads through whitespace, return true if it hits @a separator first, false if it hits @a end first.
    * If it hits anything other than these or whitespace, it return true and backpedals one char
    *
    * @tparam end The character that means we hit the end, no more to come
    * @tparam separator The character that means we have more to come
    *
    * @return returns true if we can expect more input, false if we just hit the end
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
                --p;
                return true;
            }
        }
        handleError(std::string("Expected a '") + separator + "' or a '" + end + "' but hit the end of the input");
        return false;
    }

    /**
    * @brief While reading an object .. returns true if there is more to read. Also moves us forward in the parsing.
    *
    * @return true if there is more Object to read
    */
    bool doIHaveMoreObject() {
        return doIHaveMore<'}'>();
    }

    /// Returns the pointer to the json we are parsing
    const char* json() const { return p; }

    /// This is the type you can pass to 'readObject'
    typedef std::function<void()> Reader;
    typedef std::pair<JSONType, Reader> TypeReaderPair;
    typedef std::map<std::string, TypeReaderPair> ReaderMap;

    /// Reads an object
    /// @a readerMap - a map of attribute name to expected JSONType, reader function pair.
    /// For expample: { { "name", { JSONType::string, [&parser, &person]() { person.setName(parser.readString()); } }}}
    /// @returns - the number of attributes read
    size_t readObject(const ReaderMap& readerMap) {
        size_t attrsRead = 0;
        while (doIHaveMoreObject()) {
            std::string nextAttrName = readNextAttribute();
            // See if we expected an attribute with that name
            auto pReader = readerMap.find(nextAttrName);
            if (pReader == readerMap.end()) {
                consumeOneValue();
                continue;
            }
            // See if the type matches what we expected
            JSONType nextTokenType = getNextType();
            auto pair = pReader->second;
            auto expectedType = pair.first;
            auto reader = pair.second;
            if (nextTokenType != expectedType) {
                if (nextTokenType == JSONType::null) {
                    readNull();
                    // If we expected something and found null,
                    // We'll just pretend we didn't find it
                    // TODO: possibly make the reader() smarter and able
                    //       to be notified about finding null
                    continue;
                }
                std::stringstream errMsg;
                errMsg << "When reading attribute "
                       << nextAttrName
                       << "I expected a value of type "
                       << expectedType
                       << "but got one of type "
                       << nextTokenType;
                throw JSONParserError(this, p, errMsg.str());
            }
            reader(); // Actually read in the value to the person object
            ++attrsRead;
        }
        return attrsRead;
    }
};

} // namespace yajp

#endif // YAJP_JSON_HPP
