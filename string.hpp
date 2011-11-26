
#line 1 "/home/matiu/projects/yajp/string_standalone.rl"
/** Ragel file for parsing a string in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {

// data //////////////////////////////////////////

#line 16 "/home/matiu/projects/yajp/string.hpp"
static const char _json_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 2, 7, 5, 2, 7, 
	8
};

static const int json_start = 1;
static const int json_first_final = 9;
static const int json_error = 0;

static const int json_en_main = 1;


#line 17 "/home/matiu/projects/yajp/string_standalone.rl"


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
    std::string currentString;
    currentString.reserve(expectedSize);

    
#line 54 "/home/matiu/projects/yajp/string.hpp"
	{
	cs = json_start;
	}

#line 39 "/home/matiu/projects/yajp/string_standalone.rl"
    
#line 61 "/home/matiu/projects/yajp/string.hpp"
	{
	const char *_acts;
	unsigned int _nacts;

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

	f2: _acts = _json_actions + 1; goto execFuncs;
	f3: _acts = _json_actions + 3; goto execFuncs;
	f4: _acts = _json_actions + 5; goto execFuncs;
	f5: _acts = _json_actions + 7; goto execFuncs;
	f6: _acts = _json_actions + 9; goto execFuncs;
	f0: _acts = _json_actions + 11; goto execFuncs;
	f7: _acts = _json_actions + 13; goto execFuncs;
	f10: _acts = _json_actions + 15; goto execFuncs;
	f1: _acts = _json_actions + 17; goto execFuncs;
	f8: _acts = _json_actions + 19; goto execFuncs;
	f9: _acts = _json_actions + 22; goto execFuncs;

execFuncs:
	_nacts = *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 0:
#line 4 "/home/matiu/projects/yajp/string.rl"
	{ currentString += '\b'; }
	break;
	case 1:
#line 5 "/home/matiu/projects/yajp/string.rl"
	{ currentString += '\f'; }
	break;
	case 2:
#line 6 "/home/matiu/projects/yajp/string.rl"
	{ currentString += '\n'; }
	break;
	case 3:
#line 7 "/home/matiu/projects/yajp/string.rl"
	{ currentString += '\r'; }
	break;
	case 4:
#line 8 "/home/matiu/projects/yajp/string.rl"
	{ currentString += '\t'; }
	break;
	case 5:
#line 9 "/home/matiu/projects/yajp/string.rl"
	{ currentString += *p; }
	break;
	case 6:
#line 11 "/home/matiu/projects/yajp/string.rl"
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
	break;
	case 7:
#line 21 "/home/matiu/projects/yajp/string.rl"
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
	break;
	case 8:
#line 39 "/home/matiu/projects/yajp/string.rl"
	{
        callback.foundSimpleValue(std::move(currentString));
    }
	break;
#line 243 "/home/matiu/projects/yajp/string.hpp"
		}
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

#line 40 "/home/matiu/projects/yajp/string_standalone.rl"
}

}
