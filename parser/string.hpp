
#line 1 "/Users/matiu/projects/yajp/parser/string_standalone.rl"
/** Ragel file for parsing a string in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {

// data //////////////////////////////////////////

#line 16 "/Users/matiu/projects/yajp/parser/string.hpp"
static const int json_start = 1;
static const int json_first_final = 9;
static const int json_error = 0;

static const int json_en_main = 1;


#line 17 "/Users/matiu/projects/yajp/parser/string_standalone.rl"


/**
* @brief Parse a json string
*
* @tparam T any class that has  a 'foundString' method that we can call
* @param json the json that we are parsing .. should be already at the start of the string
* @param mapper an object that we can call 'foundString(const std::string&) or foundString(std::string&&) (c++11 move) on
* @param expectedSize the expected size of the output string
*/
template <class T>
void parseString(const std::string& json, T& mapper, unsigned long expectedSize=256) {
    // Ragel vars
    int cs;
    const char *p = &json.c_str()[0];
    const char *pe = p + json.length();
    // action vars
    unsigned long uniChar = 0;
    std::string currentString;
    currentString.reserve(expectedSize);

    
#line 47 "/Users/matiu/projects/yajp/parser/string.hpp"
	{
	cs = json_start;
	}

#line 39 "/Users/matiu/projects/yajp/parser/string_standalone.rl"
    
#line 54 "/Users/matiu/projects/yajp/parser/string.hpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 34 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr2:
#line 9 "/Users/matiu/projects/yajp/parser/string.rl"
	{ currentString += *p; }
	goto st2;
tr5:
#line 4 "/Users/matiu/projects/yajp/parser/string.rl"
	{ currentString += '\b'; }
	goto st2;
tr6:
#line 5 "/Users/matiu/projects/yajp/parser/string.rl"
	{ currentString += '\f'; }
	goto st2;
tr7:
#line 6 "/Users/matiu/projects/yajp/parser/string.rl"
	{ currentString += '\n'; }
	goto st2;
tr8:
#line 7 "/Users/matiu/projects/yajp/parser/string.rl"
	{ currentString += '\r'; }
	goto st2;
tr9:
#line 8 "/Users/matiu/projects/yajp/parser/string.rl"
	{ currentString += '\t'; }
	goto st2;
tr15:
#line 21 "/Users/matiu/projects/yajp/parser/string.rl"
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
#line 9 "/Users/matiu/projects/yajp/parser/string.rl"
	{ currentString += *p; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 118 "/Users/matiu/projects/yajp/parser/string.hpp"
	switch( (*p) ) {
		case 34: goto tr3;
		case 92: goto st3;
	}
	goto tr2;
tr3:
#line 39 "/Users/matiu/projects/yajp/parser/string.rl"
	{
        mapper.foundSimpleValue(std::move(currentString));
        currentString.clear();
    }
	goto st9;
tr16:
#line 21 "/Users/matiu/projects/yajp/parser/string.rl"
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
#line 39 "/Users/matiu/projects/yajp/parser/string.rl"
	{
        mapper.foundSimpleValue(std::move(currentString));
        currentString.clear();
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 161 "/Users/matiu/projects/yajp/parser/string.hpp"
	goto st0;
tr17:
#line 21 "/Users/matiu/projects/yajp/parser/string.rl"
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
#line 188 "/Users/matiu/projects/yajp/parser/string.hpp"
	switch( (*p) ) {
		case 98: goto tr5;
		case 102: goto tr6;
		case 110: goto tr7;
		case 114: goto tr8;
		case 116: goto tr9;
		case 117: goto st4;
	}
	goto tr2;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr11;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr11;
	} else
		goto tr11;
	goto st0;
tr11:
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
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 228 "/Users/matiu/projects/yajp/parser/string.hpp"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr12;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr12;
	} else
		goto tr12;
	goto st0;
tr12:
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
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 255 "/Users/matiu/projects/yajp/parser/string.hpp"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr13;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr13;
	} else
		goto tr13;
	goto st0;
tr13:
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
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 282 "/Users/matiu/projects/yajp/parser/string.hpp"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr14;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr14;
	} else
		goto tr14;
	goto st0;
tr14:
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
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 309 "/Users/matiu/projects/yajp/parser/string.hpp"
	switch( (*p) ) {
		case 34: goto tr16;
		case 92: goto tr17;
	}
	goto tr15;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 40 "/Users/matiu/projects/yajp/parser/string_standalone.rl"
}

}
