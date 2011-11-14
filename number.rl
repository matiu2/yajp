/** Ragel file for parsing a number in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {

%%{
    # number machine
    machine number;

    # Actions
    # handle a number
    action setNegative {
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
    action recordInt {
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
    action recordDecimal {
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
    action setExpNeg {
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
    action recordExponent {
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
    }
    action gotNumber {
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
    # JSON Number expression
    basic_int = ('-'?@setNegative).([0-9]+@recordInt)@recordInt;
    basic_float = (basic_int|'-'?.'0').'.'.([0-9]+)@recordDecimal;
    exponent = [eE].([+\-]?@setExpNeg).([0-9]+)@recordExponent;
    number = (basic_int . (exponent?) | basic_float . (exponent?))%gotNumber;

    main := number;
}%%

// data //////////////////////////////////////////
%%write data;

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
    %%write init;
    // Execution of state machine
    %%write exec;
}

}
