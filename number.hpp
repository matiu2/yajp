
/* #line 1 "number.rl" */
/** Ragel file for parsing a number in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {


/* #line 83 "number.rl" */


// data //////////////////////////////////////////

/* #line 20 "number.hpp" */
static const char _number_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 6, 6, 
	6
};

static const int number_start = 1;
static const int number_first_final = 6;
static const int number_error = 0;

static const int number_en_main = 1;


/* #line 87 "number.rl" */

template <class T>
void parseNumber(const std::string& json, T& callback) {
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
    
/* #line 50 "number.hpp" */
	{
	cs = number_start;
	}

/* #line 103 "number.rl" */
    // Execution of state machine
    
/* #line 58 "number.hpp" */
	{
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

f0:
/* #line 17 "number.rl" */
	{
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
	goto _again;
f1:
/* #line 23 "number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
	goto _again;
f2:
/* #line 30 "number.rl" */
	{
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
	goto _again;
f3:
/* #line 38 "number.rl" */
	{
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
	goto _again;
f4:
/* #line 45 "number.rl" */
	{
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
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
	switch ( _number_eof_actions[cs] ) {
	case 6:
/* #line 52 "number.rl" */
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
            callback.foundNumber(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                callback.foundNumber(result2);
            } else {
                callback.foundNumber(result);
            }
        }
    }
	break;
/* #line 213 "number.hpp" */
	}
	}

	_out: {}
	}

/* #line 105 "number.rl" */
}

}
