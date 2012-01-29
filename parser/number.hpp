
/* #line 1 "/home/matiu/projects/yajp/parser/number_standalone.rl" */
/** Ragel file for parsing a number in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {

// data //////////////////////////////////////////

/* #line 16 "/home/matiu/projects/yajp/parser/number.hpp" */
static const int json_start = 1;
static const int json_first_final = 6;
static const int json_error = 0;

static const int json_en_main = 1;


/* #line 17 "/home/matiu/projects/yajp/parser/number_standalone.rl" */


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
    
/* #line 42 "/home/matiu/projects/yajp/parser/number.hpp" */
	{
	cs = json_start;
	}

/* #line 34 "/home/matiu/projects/yajp/parser/number_standalone.rl" */
    // Execution of state machine
    
/* #line 50 "/home/matiu/projects/yajp/parser/number.hpp" */
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
/* #line 7 "/home/matiu/projects/yajp/parser/number.rl" */
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
/* #line 78 "/home/matiu/projects/yajp/parser/number.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
/* #line 13 "/home/matiu/projects/yajp/parser/number.rl" */
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
/* #line 96 "/home/matiu/projects/yajp/parser/number.hpp" */
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
/* #line 20 "/home/matiu/projects/yajp/parser/number.rl" */
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
/* #line 127 "/home/matiu/projects/yajp/parser/number.hpp" */
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
/* #line 28 "/home/matiu/projects/yajp/parser/number.rl" */
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
/* #line 160 "/home/matiu/projects/yajp/parser/number.hpp" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr5:
/* #line 35 "/home/matiu/projects/yajp/parser/number.rl" */
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
/* #line 178 "/home/matiu/projects/yajp/parser/number.hpp" */
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
/* #line 224 "/home/matiu/projects/yajp/parser/number.hpp" */
	}
	}

	_out: {}
	}

/* #line 36 "/home/matiu/projects/yajp/parser/number_standalone.rl" */
}

}
