
#line 1 "number.rl"
/** Ragel file for parsing a number in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {


#line 83 "number.rl"


// data //////////////////////////////////////////

#line 20 "number.hpp"
static const int number_start = 1;
static const int number_first_final = 6;
static const int number_error = 0;

static const int number_en_main = 1;


#line 87 "number.rl"

template <class T>
void parseNumber(const std::string& json, T& callback) {
    // Ragel vars
    int cs;
    const char *p = &json.c_str()[0];
    const char *pe = p + json.length();
    const char *eof = pe;
    // action vars
    unsigned long result1; // One of these will be the result
    signed long result2;   // This one if we hit a sign but no -ve exponent or float
    double result3;        // This one if hit a -ve exponent or float
    const char *numStart = p; // Where the last number start was discovered
    bool intIsNeg=false; // true if the int part is negative
    bool expIsNeg=false; // true if the exponent part is negative
    unsigned long long intPart=0; // The integer part of the number
    long expPart1=0; // The inferred exponent part gotten from counting the decimal digits
    long expPart2=0; // The explicit exponent part from the number itself, added to the inferred exponent part
    bool isFloat = false; // is set to true if we see a '.'
    // Initialization of state machine
    
#line 50 "number.hpp"
	{
	cs = number_start;
	}

#line 108 "number.rl"
    // Execution of state machine
    
#line 58 "number.hpp"
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
#line 17 "number.rl"
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
#line 86 "number.hpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 23 "number.rl"
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
#line 104 "number.hpp"
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
#line 30 "number.rl"
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
#line 135 "number.hpp"
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
#line 38 "number.rl"
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
#line 168 "number.hpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr5:
#line 45 "number.rl"
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
#line 186 "number.hpp"
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
#line 52 "number.rl"
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
#line 232 "number.hpp"
	}
	}

	_out: {}
	}

#line 110 "number.rl"
}

}
