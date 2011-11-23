
/* #line 1 "string.rl" */
/** Ragel file for parsing a string in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {


/* #line 65 "string.rl" */


// data //////////////////////////////////////////

/* #line 20 "string.hpp" */
static const int string_start = 1;
static const int string_first_final = 9;
static const int string_error = 0;

static const int string_en_main = 1;


/* #line 69 "string.rl" */

/**
* @brief Parse a json string
*
* @tparam T any class that has  a 'foundString' method that we can call
* @param json the json that we are parsing .. should be already at the start of the string
* @param callback an object that we can call 'foundString(const std::string&) or foundString(std::string&&) (c++11 move) on
* @param expectedSize the expected size of the output string
*/
template <class T>
void parseString(const std::string& json, T& callback, unsigned long expectedSize=256) {
    // Ragel vars
    int cs;
    const char *p = &json.c_str()[0];
    const char *pe = p + json.length();
    // action vars
    unsigned long uniChar = 0;
    std::string result;
    result.reserve(expectedSize);

    
/* #line 50 "string.hpp" */
	{
	cs = string_start;
	}

/* #line 90 "string.rl" */
    
/* #line 57 "string.hpp" */
	{
	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	switch ( cs ) {
case 1:
	if ( (*p) == 34 )
		goto tr0;
	goto tr1;
case 0:
	goto _out;
case 2:
	switch( (*p) ) {
		case 34: goto tr3;
		case 92: goto tr4;
	}
	goto tr2;
case 9:
	goto tr1;
case 3:
	switch( (*p) ) {
		case 98: goto tr5;
		case 102: goto tr6;
		case 110: goto tr7;
		case 114: goto tr8;
		case 116: goto tr9;
		case 117: goto tr10;
	}
	goto tr2;
case 4:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr11;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr11;
	} else
		goto tr11;
	goto tr1;
case 5:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr12;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr12;
	} else
		goto tr12;
	goto tr1;
case 6:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr13;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr13;
	} else
		goto tr13;
	goto tr1;
case 7:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr14;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr14;
	} else
		goto tr14;
	goto tr1;
case 8:
	switch( (*p) ) {
		case 34: goto tr16;
		case 92: goto tr17;
	}
	goto tr15;
	}

	tr1: cs = 0; goto _again;
	tr0: cs = 2; goto _again;
	tr2: cs = 2; goto f0;
	tr5: cs = 2; goto f2;
	tr6: cs = 2; goto f3;
	tr7: cs = 2; goto f4;
	tr8: cs = 2; goto f5;
	tr9: cs = 2; goto f6;
	tr15: cs = 2; goto f8;
	tr4: cs = 3; goto _again;
	tr17: cs = 3; goto f10;
	tr10: cs = 4; goto _again;
	tr11: cs = 5; goto f7;
	tr12: cs = 6; goto f7;
	tr13: cs = 7; goto f7;
	tr14: cs = 8; goto f7;
	tr3: cs = 9; goto f1;
	tr16: cs = 9; goto f9;

f2:
/* #line 14 "string.rl" */
	{ result += '\b'; }
	goto _again;
f3:
/* #line 15 "string.rl" */
	{ result += '\f'; }
	goto _again;
f4:
/* #line 16 "string.rl" */
	{ result += '\n'; }
	goto _again;
f5:
/* #line 17 "string.rl" */
	{ result += '\r'; }
	goto _again;
f6:
/* #line 18 "string.rl" */
	{ result += '\t'; }
	goto _again;
f0:
/* #line 19 "string.rl" */
	{ result += *p; }
	goto _again;
f7:
/* #line 21 "string.rl" */
	{
        uniChar <<= 4;
        char ch = *p;
        if (ch >= 'a') 
            uniChar += ch - 'a' + 0xa;
        else if (ch >= 'A')
            uniChar += ch - 'A' + 0xa;
        else
            uniChar += ch - '0';
    }
	goto _again;
f10:
/* #line 31 "string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            result += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            result += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        } else if (uniChar <= 0xffff) {
            result += (uniChar >> 12) | 0xE0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        } else if (uniChar <= 0x10ffff) {
            result += (uniChar >> 18) | 0xF0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 12) & 0x3f) | 0x80; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        }
    }
	goto _again;
f1:
/* #line 49 "string.rl" */
	{
        callback.foundString(std::move(result));
    }
	goto _again;
f8:
/* #line 31 "string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            result += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            result += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        } else if (uniChar <= 0xffff) {
            result += (uniChar >> 12) | 0xE0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        } else if (uniChar <= 0x10ffff) {
            result += (uniChar >> 18) | 0xF0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 12) & 0x3f) | 0x80; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        }
    }
/* #line 19 "string.rl" */
	{ result += *p; }
	goto _again;
f9:
/* #line 31 "string.rl" */
	{
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            result += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            result += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        } else if (uniChar <= 0xffff) {
            result += (uniChar >> 12) | 0xE0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        } else if (uniChar <= 0x10ffff) {
            result += (uniChar >> 18) | 0xF0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 12) & 0x3f) | 0x80; // 110 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        }
    }
/* #line 49 "string.rl" */
	{
        callback.foundString(std::move(result));
    }
	goto _again;

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

/* #line 91 "string.rl" */
}

}
