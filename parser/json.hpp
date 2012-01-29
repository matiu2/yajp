
/* #line 1 "/home/matiu/projects/yajp/parser/json.rl" */
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


/* #line 77 "/home/matiu/projects/yajp/parser/json.rl" */


const int STACK_JUMP_SIZE=16384; // How much memory in 'ints' to get each time the yajp stack needs an update

class JSONParser {
private:

    // Ragel vars
    int cs; // Current state
    const char *p;
    const char *pe;
    const char *eof;

public:
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
        // All the bits it needs
        
/* #line 59 "/home/matiu/projects/yajp/parser/json.hpp" */
static const int json_start = 1;
static const int json_first_final = 122;
static const int json_error = 0;

static const int json_en_main = 1;
static const int json_en_array = 23;
static const int json_en_object = 50;


/* #line 115 "/home/matiu/projects/yajp/parser/json.rl" */
        // Initialization of state machine
        
/* #line 72 "/home/matiu/projects/yajp/parser/json.hpp" */
	{
	cs = json_start;
	top = 0;
	}

/* #line 117 "/home/matiu/projects/yajp/parser/json.rl" */
        // Execution of state machine
        
/* #line 81 "/home/matiu/projects/yajp/parser/json.hpp" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 32: goto st1;
		case 34: goto st2;
		case 45: goto tr3;
		case 91: goto tr5;
		case 102: goto st13;
		case 110: goto st17;
		case 116: goto st20;
		case 123: goto tr9;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr4;
	} else if ( (*p) >= 9 )
		goto st1;
	goto st0;
st0:
cs = 0;
	goto _out;
tr10:
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st2;
tr13:
/* #line 4 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\b'; }
	goto st2;
tr14:
/* #line 5 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\f'; }
	goto st2;
tr15:
/* #line 6 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\n'; }
	goto st2;
tr16:
/* #line 7 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\r'; }
	goto st2;
tr17:
/* #line 8 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\t'; }
	goto st2;
tr23:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
/* #line 161 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr11;
		case 92: goto st3;
	}
	goto tr10;
tr5:
/* #line 25 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we hit the start of an array
        // The Array is recursive, as it can hold further arrays, so we need to employ our fake stack and use fcall
        mapper.startArray();
        {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    {stack[top++] = 122; goto st23;}}
    }
	goto st122;
tr9:
/* #line 34 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we hit the start of an object '{' starts recursing to get the inside parts of an object
        // The Object is recursive, as it can hold further objects, so we need to employ our fake stack and use fcall
        mapper.startObj();
        {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    {stack[top++] = 122; goto st50;}}
    }
	goto st122;
tr11:
/* #line 39 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        mapper.foundSimpleValue(std::move(currentString));
    }
	goto st122;
tr24:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 39 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        mapper.foundSimpleValue(std::move(currentString));
    }
	goto st122;
tr32:
/* #line 23 "/home/matiu/projects/yajp/parser/json.rl" */
	{ mapper.foundSimpleValue(false); }
	goto st122;
tr38:
/* #line 22 "/home/matiu/projects/yajp/parser/json.rl" */
	{ mapper.foundSimpleValue(true); }
	goto st122;
tr210:
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
/* #line 265 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) == 32 )
		goto st122;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st122;
	goto st0;
tr25:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
/* #line 296 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 98: goto tr13;
		case 102: goto tr14;
		case 110: goto tr15;
		case 114: goto tr16;
		case 116: goto tr17;
		case 117: goto st4;
	}
	goto tr10;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr19;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr19;
	} else
		goto tr19;
	goto st0;
tr19:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
/* #line 336 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr20;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr20;
	} else
		goto tr20;
	goto st0;
tr20:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
/* #line 363 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr21;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr21;
	} else
		goto tr21;
	goto st0;
tr21:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
/* #line 390 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr22;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr22;
	} else
		goto tr22;
	goto st0;
tr22:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
/* #line 417 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr24;
		case 92: goto tr25;
	}
	goto tr23;
tr3:
/* #line 7 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
/* #line 436 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto st0;
tr4:
/* #line 13 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
	goto st123;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
/* #line 454 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr210;
		case 46: goto st10;
		case 69: goto st11;
		case 101: goto st11;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr4;
	} else if ( (*p) >= 9 )
		goto tr210;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr26:
/* #line 20 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
	goto st124;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
/* #line 489 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr210;
		case 69: goto st11;
		case 101: goto st11;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr26;
	} else if ( (*p) >= 9 )
		goto tr210;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 43: goto tr27;
		case 45: goto tr27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr27:
/* #line 28 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
/* #line 526 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr28:
/* #line 35 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
    }
	goto st125;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
/* #line 544 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) == 32 )
		goto tr210;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr28;
	} else if ( (*p) >= 9 )
		goto tr210;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 97 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 108 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 115 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 101 )
		goto tr32;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 117 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 108 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 108 )
		goto st122;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 114 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 117 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 101 )
		goto tr38;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 32: goto st23;
		case 34: goto st24;
		case 45: goto tr41;
		case 91: goto tr43;
		case 93: goto tr44;
		case 102: goto st34;
		case 110: goto st38;
		case 116: goto st41;
		case 123: goto tr48;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr42;
	} else if ( (*p) >= 9 )
		goto st23;
	goto st0;
tr49:
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st24;
tr71:
/* #line 4 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\b'; }
	goto st24;
tr72:
/* #line 5 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\f'; }
	goto st24;
tr73:
/* #line 6 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\n'; }
	goto st24;
tr74:
/* #line 7 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\r'; }
	goto st24;
tr75:
/* #line 8 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\t'; }
	goto st24;
tr81:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
/* #line 695 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr50;
		case 92: goto st44;
	}
	goto tr49;
tr43:
/* #line 25 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we hit the start of an array
        // The Array is recursive, as it can hold further arrays, so we need to employ our fake stack and use fcall
        mapper.startArray();
        {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    {stack[top++] = 25; goto st23;}}
    }
	goto st25;
tr48:
/* #line 34 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we hit the start of an object '{' starts recursing to get the inside parts of an object
        // The Object is recursive, as it can hold further objects, so we need to employ our fake stack and use fcall
        mapper.startObj();
        {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    {stack[top++] = 25; goto st50;}}
    }
	goto st25;
tr50:
/* #line 39 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        mapper.foundSimpleValue(std::move(currentString));
    }
	goto st25;
tr82:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 39 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        mapper.foundSimpleValue(std::move(currentString));
    }
	goto st25;
tr65:
/* #line 23 "/home/matiu/projects/yajp/parser/json.rl" */
	{ mapper.foundSimpleValue(false); }
	goto st25;
tr70:
/* #line 22 "/home/matiu/projects/yajp/parser/json.rl" */
	{ mapper.foundSimpleValue(true); }
	goto st25;
tr54:
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
/* #line 799 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto st25;
		case 44: goto st26;
		case 93: goto tr44;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st25;
	goto st0;
tr55:
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
/* #line 839 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto st26;
		case 34: goto st24;
		case 45: goto tr41;
		case 91: goto tr43;
		case 102: goto st34;
		case 110: goto st38;
		case 116: goto st41;
		case 123: goto tr48;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr42;
	} else if ( (*p) >= 9 )
		goto st26;
	goto st0;
tr41:
/* #line 7 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
/* #line 869 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr42:
/* #line 13 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
/* #line 887 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr54;
		case 44: goto tr55;
		case 46: goto st29;
		case 69: goto st31;
		case 93: goto tr58;
		case 101: goto st31;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr42;
	} else if ( (*p) >= 9 )
		goto tr54;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr59;
	goto st0;
tr59:
/* #line 20 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
/* #line 924 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr54;
		case 44: goto tr55;
		case 69: goto st31;
		case 93: goto tr58;
		case 101: goto st31;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr59;
	} else if ( (*p) >= 9 )
		goto tr54;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 43: goto tr60;
		case 45: goto tr60;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr61;
	goto st0;
tr60:
/* #line 28 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
/* #line 963 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr61;
	goto st0;
tr61:
/* #line 35 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
    }
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
/* #line 981 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr54;
		case 44: goto tr55;
		case 93: goto tr58;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr61;
	} else if ( (*p) >= 9 )
		goto tr54;
	goto st0;
tr44:
/* #line 31 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        mapper.endArray();
    }
	goto st126;
tr58:
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
/* #line 31 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        mapper.endArray();
    }
	goto st126;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
/* #line 1034 "/home/matiu/projects/yajp/parser/json.hpp" */
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 97 )
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 108 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 115 )
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 101 )
		goto tr65;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 117 )
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 108 )
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 108 )
		goto st25;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 114 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 117 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 101 )
		goto tr70;
	goto st0;
tr83:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
/* #line 1131 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 98: goto tr71;
		case 102: goto tr72;
		case 110: goto tr73;
		case 114: goto tr74;
		case 116: goto tr75;
		case 117: goto st45;
	}
	goto tr49;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr77;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr77;
	} else
		goto tr77;
	goto st0;
tr77:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
/* #line 1171 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr78;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr78;
	} else
		goto tr78;
	goto st0;
tr78:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
/* #line 1198 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr79;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr79;
	} else
		goto tr79;
	goto st0;
tr79:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
/* #line 1225 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr80;
	} else
		goto tr80;
	goto st0;
tr80:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
/* #line 1252 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr82;
		case 92: goto tr83;
	}
	goto tr81;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 32: goto st50;
		case 34: goto st51;
		case 125: goto tr86;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st50;
	goto st0;
tr87:
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st51;
tr197:
/* #line 4 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\b'; }
	goto st51;
tr198:
/* #line 5 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\f'; }
	goto st51;
tr199:
/* #line 6 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\n'; }
	goto st51;
tr200:
/* #line 7 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\r'; }
	goto st51;
tr201:
/* #line 8 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\t'; }
	goto st51;
tr207:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
/* #line 1321 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr88;
		case 92: goto st116;
	}
	goto tr87;
tr88:
/* #line 43 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we found a new property for a value
        mapper.propertyName(std::move(currentString));
    }
	goto st52;
tr208:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 43 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we found a new property for a value
        mapper.propertyName(std::move(currentString));
    }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
/* #line 1364 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto st52;
		case 58: goto st53;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st52;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 32: goto st53;
		case 34: goto st54;
		case 45: goto tr93;
		case 91: goto tr95;
		case 102: goto st106;
		case 110: goto st110;
		case 116: goto st113;
		case 123: goto tr99;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr94;
	} else if ( (*p) >= 9 )
		goto st53;
	goto st0;
tr100:
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st54;
tr168:
/* #line 4 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\b'; }
	goto st54;
tr169:
/* #line 5 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\f'; }
	goto st54;
tr170:
/* #line 6 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\n'; }
	goto st54;
tr171:
/* #line 7 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\r'; }
	goto st54;
tr172:
/* #line 8 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\t'; }
	goto st54;
tr178:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
/* #line 1443 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr101;
		case 92: goto st93;
	}
	goto tr100;
tr95:
/* #line 25 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we hit the start of an array
        // The Array is recursive, as it can hold further arrays, so we need to employ our fake stack and use fcall
        mapper.startArray();
        {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    {stack[top++] = 55; goto st23;}}
    }
	goto st55;
tr99:
/* #line 34 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we hit the start of an object '{' starts recursing to get the inside parts of an object
        // The Object is recursive, as it can hold further objects, so we need to employ our fake stack and use fcall
        mapper.startObj();
        {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    {stack[top++] = 55; goto st50;}}
    }
	goto st55;
tr101:
/* #line 39 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        mapper.foundSimpleValue(std::move(currentString));
    }
	goto st55;
tr179:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 39 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        mapper.foundSimpleValue(std::move(currentString));
    }
	goto st55;
tr191:
/* #line 23 "/home/matiu/projects/yajp/parser/json.rl" */
	{ mapper.foundSimpleValue(false); }
	goto st55;
tr196:
/* #line 22 "/home/matiu/projects/yajp/parser/json.rl" */
	{ mapper.foundSimpleValue(true); }
	goto st55;
tr181:
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
/* #line 1547 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto st55;
		case 44: goto st56;
		case 125: goto tr105;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st55;
	goto st0;
tr182:
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
/* #line 1587 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto st57;
		case 34: goto st51;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 32: goto st58;
		case 34: goto st51;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st58;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 32: goto st58;
		case 34: goto st59;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st58;
	goto st0;
tr109:
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st59;
tr155:
/* #line 4 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\b'; }
	goto st59;
tr156:
/* #line 5 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\f'; }
	goto st59;
tr157:
/* #line 6 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\n'; }
	goto st59;
tr158:
/* #line 7 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\r'; }
	goto st59;
tr159:
/* #line 8 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\t'; }
	goto st59;
tr165:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st59;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
/* #line 1668 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr110;
		case 92: goto st87;
	}
	goto tr109;
tr110:
/* #line 43 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we found a new property for a value
        mapper.propertyName(std::move(currentString));
    }
	goto st60;
tr166:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 43 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we found a new property for a value
        mapper.propertyName(std::move(currentString));
    }
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
/* #line 1711 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto st60;
		case 58: goto st61;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st60;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 32: goto st61;
		case 34: goto st62;
		case 45: goto tr115;
		case 91: goto tr117;
		case 102: goto st77;
		case 110: goto st81;
		case 116: goto st84;
		case 123: goto tr121;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr116;
	} else if ( (*p) >= 9 )
		goto st61;
	goto st0;
tr122:
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st62;
tr126:
/* #line 4 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\b'; }
	goto st62;
tr127:
/* #line 5 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\f'; }
	goto st62;
tr128:
/* #line 6 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\n'; }
	goto st62;
tr129:
/* #line 7 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\r'; }
	goto st62;
tr130:
/* #line 8 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += '\t'; }
	goto st62;
tr136:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 9 "/home/matiu/projects/yajp/parser/string.rl" */
	{ currentString += *p; }
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
/* #line 1790 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr123;
		case 92: goto st64;
	}
	goto tr122;
tr117:
/* #line 25 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we hit the start of an array
        // The Array is recursive, as it can hold further arrays, so we need to employ our fake stack and use fcall
        mapper.startArray();
        {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    {stack[top++] = 63; goto st23;}}
    }
	goto st63;
tr121:
/* #line 34 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        // Called when we hit the start of an object '{' starts recursing to get the inside parts of an object
        // The Object is recursive, as it can hold further objects, so we need to employ our fake stack and use fcall
        mapper.startObj();
        {
        if (stack == 0)
            stack = (int*)malloc(STACK_JUMP_SIZE);
        else if (top % STACK_JUMP_SIZE == 0)
            stack = (int*)realloc(stack, (top+STACK_JUMP_SIZE) * sizeof(int));
    {stack[top++] = 63; goto st50;}}
    }
	goto st63;
tr123:
/* #line 39 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        mapper.foundSimpleValue(std::move(currentString));
    }
	goto st63;
tr137:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
/* #line 39 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        mapper.foundSimpleValue(std::move(currentString));
    }
	goto st63;
tr149:
/* #line 23 "/home/matiu/projects/yajp/parser/json.rl" */
	{ mapper.foundSimpleValue(false); }
	goto st63;
tr154:
/* #line 22 "/home/matiu/projects/yajp/parser/json.rl" */
	{ mapper.foundSimpleValue(true); }
	goto st63;
tr139:
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
/* #line 1894 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto st63;
		case 125: goto tr105;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st63;
	goto st0;
tr105:
/* #line 40 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        mapper.endObj();
    }
	goto st127;
tr142:
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
/* #line 40 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        mapper.endObj();
    }
	goto st127;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
/* #line 1943 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) == 32 )
		goto tr105;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr105;
	goto st0;
tr138:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
/* #line 1974 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 98: goto tr126;
		case 102: goto tr127;
		case 110: goto tr128;
		case 114: goto tr129;
		case 116: goto tr130;
		case 117: goto st65;
	}
	goto tr122;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr132;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr132;
	} else
		goto tr132;
	goto st0;
tr132:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st66;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
/* #line 2014 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr133;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr133;
	} else
		goto tr133;
	goto st0;
tr133:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
/* #line 2041 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr134;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr134;
	} else
		goto tr134;
	goto st0;
tr134:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
/* #line 2068 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr135;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr135;
	} else
		goto tr135;
	goto st0;
tr135:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
/* #line 2095 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr137;
		case 92: goto tr138;
	}
	goto tr136;
tr115:
/* #line 7 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
	goto st70;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
/* #line 2114 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr116;
	goto st0;
tr116:
/* #line 13 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
/* #line 2132 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr139;
		case 46: goto st72;
		case 69: goto st74;
		case 101: goto st74;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr116;
	} else if ( (*p) >= 9 )
		goto tr139;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr143;
	goto st0;
tr143:
/* #line 20 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
/* #line 2168 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr139;
		case 69: goto st74;
		case 101: goto st74;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr143;
	} else if ( (*p) >= 9 )
		goto tr139;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 43: goto tr144;
		case 45: goto tr144;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr145;
	goto st0;
tr144:
/* #line 28 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
	goto st75;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
/* #line 2206 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr145;
	goto st0;
tr145:
/* #line 35 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
    }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
/* #line 2224 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr139;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr145;
	} else if ( (*p) >= 9 )
		goto tr139;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) == 97 )
		goto st78;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 108 )
		goto st79;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 115 )
		goto st80;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	if ( (*p) == 101 )
		goto tr149;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( (*p) == 117 )
		goto st82;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 108 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 108 )
		goto st63;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 114 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 117 )
		goto st86;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 101 )
		goto tr154;
	goto st0;
tr167:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
/* #line 2330 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 98: goto tr155;
		case 102: goto tr156;
		case 110: goto tr157;
		case 114: goto tr158;
		case 116: goto tr159;
		case 117: goto st88;
	}
	goto tr109;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr161;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr161;
	} else
		goto tr161;
	goto st0;
tr161:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
/* #line 2370 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr162;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr162;
	} else
		goto tr162;
	goto st0;
tr162:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
/* #line 2397 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr163;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr163;
	} else
		goto tr163;
	goto st0;
tr163:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
/* #line 2424 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr164;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr164;
	} else
		goto tr164;
	goto st0;
tr164:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
/* #line 2451 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr166;
		case 92: goto tr167;
	}
	goto tr165;
tr180:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
/* #line 2482 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 98: goto tr168;
		case 102: goto tr169;
		case 110: goto tr170;
		case 114: goto tr171;
		case 116: goto tr172;
		case 117: goto st94;
	}
	goto tr100;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr174;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr174;
	} else
		goto tr174;
	goto st0;
tr174:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
/* #line 2522 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr175;
	} else
		goto tr175;
	goto st0;
tr175:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
/* #line 2549 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr176;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr176;
	} else
		goto tr176;
	goto st0;
tr176:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
/* #line 2576 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr177;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr177;
	} else
		goto tr177;
	goto st0;
tr177:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
/* #line 2603 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr179;
		case 92: goto tr180;
	}
	goto tr178;
tr93:
/* #line 7 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
	goto st99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
/* #line 2622 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr94;
	goto st0;
tr94:
/* #line 13 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
/* #line 2640 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr181;
		case 44: goto tr182;
		case 46: goto st101;
		case 69: goto st103;
		case 101: goto st103;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr94;
	} else if ( (*p) >= 9 )
		goto tr181;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr185;
	goto st0;
tr185:
/* #line 20 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
	goto st102;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
/* #line 2677 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr181;
		case 44: goto tr182;
		case 69: goto st103;
		case 101: goto st103;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr185;
	} else if ( (*p) >= 9 )
		goto tr181;
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	switch( (*p) ) {
		case 43: goto tr186;
		case 45: goto tr186;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr187;
	goto st0;
tr186:
/* #line 28 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
	goto st104;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
/* #line 2716 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr187;
	goto st0;
tr187:
/* #line 35 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
    }
	goto st105;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
/* #line 2734 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 32: goto tr181;
		case 44: goto tr182;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr187;
	} else if ( (*p) >= 9 )
		goto tr181;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 97 )
		goto st107;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	if ( (*p) == 108 )
		goto st108;
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 115 )
		goto st109;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	if ( (*p) == 101 )
		goto tr191;
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 117 )
		goto st111;
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 108 )
		goto st112;
	goto st0;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	if ( (*p) == 108 )
		goto st55;
	goto st0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	if ( (*p) == 114 )
		goto st114;
	goto st0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	if ( (*p) == 117 )
		goto st115;
	goto st0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	if ( (*p) == 101 )
		goto tr196;
	goto st0;
tr209:
/* #line 21 "/home/matiu/projects/yajp/parser/string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            currentString += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            currentString += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            currentString += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            currentString += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            currentString += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            currentString += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
    }
	goto st116;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
/* #line 2841 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 98: goto tr197;
		case 102: goto tr198;
		case 110: goto tr199;
		case 114: goto tr200;
		case 116: goto tr201;
		case 117: goto st117;
	}
	goto tr87;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr203;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr203;
	} else
		goto tr203;
	goto st0;
tr203:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st118;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
/* #line 2881 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr204;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr204;
	} else
		goto tr204;
	goto st0;
tr204:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st119;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
/* #line 2908 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr205;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr205;
	} else
		goto tr205;
	goto st0;
tr205:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st120;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
/* #line 2935 "/home/matiu/projects/yajp/parser/json.hpp" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr206;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr206;
	} else
		goto tr206;
	goto st0;
tr206:
/* #line 11 "/home/matiu/projects/yajp/parser/string.rl" */
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
	goto st121;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
/* #line 2962 "/home/matiu/projects/yajp/parser/json.hpp" */
	switch( (*p) ) {
		case 34: goto tr208;
		case 92: goto tr209;
	}
	goto tr207;
tr86:
/* #line 40 "/home/matiu/projects/yajp/parser/json.rl" */
	{
        mapper.endObj();
    }
	goto st128;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
/* #line 2978 "/home/matiu/projects/yajp/parser/json.hpp" */
	goto st0;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 123: 
	case 124: 
	case 125: 
/* #line 42 "/home/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "gotNumber " << expIsNeg << " - " << expPart1 << " - " << expPart2 << " - " << intPart << " - " << intIsNeg << " - ";
        #endif
        long expPart = expIsNeg ? expPart1 - expPart2 : expPart1 + expPart2;
        if (expPart < 0) {
            double result = intPart;
            while (expPart++ < 0)
                result *= 0.1;
            if (intIsNeg)
                result = -result;
            mapper.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                mapper.foundSimpleValue(result2);
            } else {
                mapper.foundSimpleValue(result);
            }
        }
    }
	break;
/* #line 3143 "/home/matiu/projects/yajp/parser/json.hpp" */
	}
	}

	_out: {}
	}

/* #line 119 "/home/matiu/projects/yajp/parser/json.rl" */
    }
};


} // namespace yajp

#endif // YAJP_JSON_HPP
