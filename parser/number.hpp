
/* #line 1 "/Users/matiu/projects/yajp/parser/number_standalone.rl" */
/** Ragel file for parsing a number in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {

// data //////////////////////////////////////////

/* #line 16 "/Users/matiu/projects/yajp/parser/number.hpp" */
static const char _json_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5
};

static const char _json_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 11, 11, 
	11
};

static const int json_start = 1;
static const int json_first_final = 6;
static const int json_error = 0;

static const int json_en_main = 1;


/* #line 17 "/Users/matiu/projects/yajp/parser/number_standalone.rl" */


template <class T>
void parseNumber(const std::string& json, T& mapper) {
    // Ragel vars
    int cs;
    const char *p = &json.c_str()[0];
    const char *pe = p + json.length();
    const char *eof = pe;
    // action vars
    bool intIsNeg=false; // true if the int part is negative
    bool expIsNeg=false; // true if the exponent part is negative
    unsigned long long intPart=0; // The integer part of the number
    long expPart1=0; // The inferred exponent part gotten from counting the decimal digits
    long expPart2=0; // The explicit exponent part from the number itself, added to the inferred exponent part
    // Initialization of state machine
    
/* #line 52 "/Users/matiu/projects/yajp/parser/number.hpp" */
	{
	cs = json_start;
	}

/* #line 34 "/Users/matiu/projects/yajp/parser/number_standalone.rl" */
    // Execution of state machine
    
/* #line 60 "/Users/matiu/projects/yajp/parser/number.hpp" */
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
	if ( (*p) == 45 )
		goto tr0;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto tr1;
case 0:
	goto _out;
case 2:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto tr1;
case 6:
	switch( (*p) ) {
		case 46: goto tr6;
		case 69: goto tr7;
		case 101: goto tr7;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto tr1;
case 3:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto tr1;
case 7:
	switch( (*p) ) {
		case 69: goto tr7;
		case 101: goto tr7;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto tr1;
case 4:
	switch( (*p) ) {
		case 43: goto tr4;
		case 45: goto tr4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto tr1;
case 5:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto tr1;
case 8:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto tr1;
	}

	tr1: cs = 0; goto _again;
	tr0: cs = 2; goto f0;
	tr6: cs = 3; goto _again;
	tr7: cs = 4; goto _again;
	tr4: cs = 5; goto f3;
	tr2: cs = 6; goto f1;
	tr3: cs = 7; goto f2;
	tr5: cs = 8; goto f4;

	f0: _acts = _json_actions + 1; goto execFuncs;
	f1: _acts = _json_actions + 3; goto execFuncs;
	f2: _acts = _json_actions + 5; goto execFuncs;
	f3: _acts = _json_actions + 7; goto execFuncs;
	f4: _acts = _json_actions + 9; goto execFuncs;

execFuncs:
	_nacts = *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 0:
/* #line 7 "/Users/matiu/projects/yajp/parser/number.rl" */
	{
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
	break;
	case 1:
/* #line 13 "/Users/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
	break;
	case 2:
/* #line 20 "/Users/matiu/projects/yajp/parser/number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
	break;
	case 3:
/* #line 28 "/Users/matiu/projects/yajp/parser/number.rl" */
	{
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
	break;
	case 4:
/* #line 35 "/Users/matiu/projects/yajp/parser/number.rl" */
	{
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
    }
	break;
/* #line 191 "/Users/matiu/projects/yajp/parser/number.hpp" */
		}
	}
	goto _again;

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _json_actions + _json_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 5:
/* #line 42 "/Users/matiu/projects/yajp/parser/number.rl" */
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
        intPart = expPart1 = expPart2 = 0;
        expIsNeg = false;
    }
	break;
/* #line 237 "/Users/matiu/projects/yajp/parser/number.hpp" */
		}
	}
	}

	_out: {}
	}

/* #line 36 "/Users/matiu/projects/yajp/parser/number_standalone.rl" */
}

}
