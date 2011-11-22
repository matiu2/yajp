
/* #line 1 "string.rl" */
/** Ragel file for parsing a string in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {


/* #line 50 "string.rl" */


// data //////////////////////////////////////////

/* #line 20 "string.hpp" */
static const int string_start = 1;
static const int string_first_final = 9;
static const int string_error = 0;

static const int string_en_main = 1;


/* #line 54 "string.rl" */

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
    //const char *eof = pe;
    // action vars
    //const char* stringStart = p;
    wchar_t uniChar = 0;
    std::string result;
    result.reserve(expectedSize);

    
/* #line 52 "string.hpp" */
	{
	cs = string_start;
	}

/* #line 77 "string.rl" */
    
/* #line 59 "string.hpp" */
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
/* #line 19 "string.rl" */
	{ result += *p; }
	goto st2;
tr5:
/* #line 14 "string.rl" */
	{ result += '\b'; }
	goto st2;
tr6:
/* #line 15 "string.rl" */
	{ result += '\f'; }
	goto st2;
tr7:
/* #line 16 "string.rl" */
	{ result += '\n'; }
	goto st2;
tr8:
/* #line 17 "string.rl" */
	{ result += '\r'; }
	goto st2;
tr9:
/* #line 18 "string.rl" */
	{ result += '\t'; }
	goto st2;
tr12:
/* #line 31 "string.rl" */
	{
        result += uniChar;
    }
/* #line 19 "string.rl" */
	{ result += *p; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
/* #line 108 "string.hpp" */
	switch( (*p) ) {
		case 34: goto tr3;
		case 92: goto st3;
	}
	goto tr2;
tr3:
/* #line 34 "string.rl" */
	{
        callback.foundString(std::move(result));
    }
	goto st9;
tr13:
/* #line 31 "string.rl" */
	{
        result += uniChar;
    }
/* #line 34 "string.rl" */
	{
        callback.foundString(std::move(result));
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
/* #line 134 "string.hpp" */
	goto st0;
tr15:
/* #line 31 "string.rl" */
	{
        result += uniChar;
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
/* #line 146 "string.hpp" */
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
/* #line 21 "string.rl" */
	{
        uniChar <<= 4;
        char ch = *p;
        if (ch > 'a') 
            uniChar += ch - 'a' + 0xa;
        else if (ch > 'A')
            uniChar += ch - 'A' + 0xa;
        else
            uniChar += ch - '0';
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
/* #line 186 "string.hpp" */
	switch( (*p) ) {
		case 34: goto tr13;
		case 92: goto tr15;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr14;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr14;
	} else
		goto tr14;
	goto tr12;
tr14:
/* #line 21 "string.rl" */
	{
        uniChar <<= 4;
        char ch = *p;
        if (ch > 'a') 
            uniChar += ch - 'a' + 0xa;
        else if (ch > 'A')
            uniChar += ch - 'A' + 0xa;
        else
            uniChar += ch - '0';
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
/* #line 217 "string.hpp" */
	switch( (*p) ) {
		case 34: goto tr13;
		case 92: goto tr15;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr16;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr16;
	} else
		goto tr16;
	goto tr12;
tr16:
/* #line 21 "string.rl" */
	{
        uniChar <<= 4;
        char ch = *p;
        if (ch > 'a') 
            uniChar += ch - 'a' + 0xa;
        else if (ch > 'A')
            uniChar += ch - 'A' + 0xa;
        else
            uniChar += ch - '0';
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
/* #line 248 "string.hpp" */
	switch( (*p) ) {
		case 34: goto tr13;
		case 92: goto tr15;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr17;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr17;
	} else
		goto tr17;
	goto tr12;
tr17:
/* #line 21 "string.rl" */
	{
        uniChar <<= 4;
        char ch = *p;
        if (ch > 'a') 
            uniChar += ch - 'a' + 0xa;
        else if (ch > 'A')
            uniChar += ch - 'A' + 0xa;
        else
            uniChar += ch - '0';
    }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
/* #line 279 "string.hpp" */
	switch( (*p) ) {
		case 34: goto tr13;
		case 92: goto tr15;
	}
	goto tr12;
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

/* #line 78 "string.rl" */
}

}
