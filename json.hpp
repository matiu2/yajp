
#line 1 "/home/matiu/projects/yajp/json.rl"
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


#line 61 "/home/matiu/projects/yajp/json.rl"



#line 24 "/home/matiu/projects/yajp/json.hpp"
static const char _json_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 1, 17, 1, 18, 1, 
	19, 2, 7, 5, 2, 7, 8, 2, 
	7, 19
};

static const char _json_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 29, 29, 29, 
	0, 0, 0
};

static const int json_start = 1;
static const int json_first_final = 124;
static const int json_error = 0;

static const int json_en_main = 1;
static const int json_en_array = 23;
static const int json_en_object = 51;


#line 64 "/home/matiu/projects/yajp/json.rl"

template <class T>
void parseJSON(const std::string& json, T& callback) {
    // Ragel vars
    int cs;
    const char *p = &json.c_str()[0];
    const char *pe = p + json.length();
    const char *eof = pe;
    std::vector<int> stack; // fake stack for ragel's fcall
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
    // Initialization of state machine
    
#line 86 "/home/matiu/projects/yajp/json.hpp"
	{
	cs = json_start;
	top = 0;
	}

#line 85 "/home/matiu/projects/yajp/json.rl"
    // Execution of state machine
    
#line 95 "/home/matiu/projects/yajp/json.hpp"
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
	switch( (*p) ) {
		case 32: goto tr0;
		case 34: goto tr2;
		case 45: goto tr3;
		case 91: goto tr5;
		case 102: goto tr6;
		case 110: goto tr7;
		case 116: goto tr8;
		case 123: goto tr9;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr4;
	} else if ( (*p) >= 9 )
		goto tr0;
	goto tr1;
case 0:
	goto _out;
case 2:
	switch( (*p) ) {
		case 34: goto tr11;
		case 92: goto tr12;
	}
	goto tr10;
case 124:
	if ( (*p) == 32 )
		goto tr35;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr35;
	goto tr1;
case 3:
	switch( (*p) ) {
		case 98: goto tr13;
		case 102: goto tr14;
		case 110: goto tr15;
		case 114: goto tr16;
		case 116: goto tr17;
		case 117: goto tr18;
	}
	goto tr10;
case 4:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr19;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr19;
	} else
		goto tr19;
	goto tr1;
case 5:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr20;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr20;
	} else
		goto tr20;
	goto tr1;
case 6:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr21;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr21;
	} else
		goto tr21;
	goto tr1;
case 7:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr22;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr22;
	} else
		goto tr22;
	goto tr1;
case 8:
	switch( (*p) ) {
		case 34: goto tr24;
		case 92: goto tr25;
	}
	goto tr23;
case 9:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto tr1;
case 125:
	switch( (*p) ) {
		case 32: goto tr210;
		case 46: goto tr211;
		case 69: goto tr212;
		case 101: goto tr212;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr4;
	} else if ( (*p) >= 9 )
		goto tr210;
	goto tr1;
case 10:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto tr1;
case 126:
	switch( (*p) ) {
		case 32: goto tr210;
		case 69: goto tr212;
		case 101: goto tr212;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr26;
	} else if ( (*p) >= 9 )
		goto tr210;
	goto tr1;
case 11:
	switch( (*p) ) {
		case 43: goto tr27;
		case 45: goto tr27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto tr1;
case 12:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto tr1;
case 127:
	if ( (*p) == 32 )
		goto tr210;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr28;
	} else if ( (*p) >= 9 )
		goto tr210;
	goto tr1;
case 13:
	if ( (*p) == 97 )
		goto tr29;
	goto tr1;
case 14:
	if ( (*p) == 108 )
		goto tr30;
	goto tr1;
case 15:
	if ( (*p) == 115 )
		goto tr31;
	goto tr1;
case 16:
	if ( (*p) == 101 )
		goto tr32;
	goto tr1;
case 17:
	if ( (*p) == 117 )
		goto tr33;
	goto tr1;
case 18:
	if ( (*p) == 108 )
		goto tr34;
	goto tr1;
case 19:
	if ( (*p) == 108 )
		goto tr35;
	goto tr1;
case 20:
	if ( (*p) == 114 )
		goto tr36;
	goto tr1;
case 21:
	if ( (*p) == 117 )
		goto tr37;
	goto tr1;
case 22:
	if ( (*p) == 101 )
		goto tr38;
	goto tr1;
case 23:
	if ( (*p) == 91 )
		goto tr39;
	goto tr1;
case 24:
	switch( (*p) ) {
		case 32: goto tr39;
		case 34: goto tr40;
		case 45: goto tr41;
		case 91: goto tr43;
		case 93: goto tr44;
		case 102: goto tr45;
		case 110: goto tr46;
		case 116: goto tr47;
		case 123: goto tr48;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr42;
	} else if ( (*p) >= 9 )
		goto tr39;
	goto tr1;
case 25:
	switch( (*p) ) {
		case 34: goto tr50;
		case 92: goto tr51;
	}
	goto tr49;
case 26:
	switch( (*p) ) {
		case 32: goto tr52;
		case 44: goto tr53;
		case 93: goto tr44;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr52;
	goto tr1;
case 27:
	switch( (*p) ) {
		case 32: goto tr53;
		case 34: goto tr40;
		case 45: goto tr41;
		case 91: goto tr43;
		case 102: goto tr45;
		case 110: goto tr46;
		case 116: goto tr47;
		case 123: goto tr48;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr42;
	} else if ( (*p) >= 9 )
		goto tr53;
	goto tr1;
case 28:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto tr1;
case 29:
	switch( (*p) ) {
		case 32: goto tr54;
		case 44: goto tr55;
		case 46: goto tr56;
		case 69: goto tr57;
		case 93: goto tr58;
		case 101: goto tr57;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr42;
	} else if ( (*p) >= 9 )
		goto tr54;
	goto tr1;
case 30:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr59;
	goto tr1;
case 31:
	switch( (*p) ) {
		case 32: goto tr54;
		case 44: goto tr55;
		case 69: goto tr57;
		case 93: goto tr58;
		case 101: goto tr57;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr59;
	} else if ( (*p) >= 9 )
		goto tr54;
	goto tr1;
case 32:
	switch( (*p) ) {
		case 43: goto tr60;
		case 45: goto tr60;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr61;
	goto tr1;
case 33:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr61;
	goto tr1;
case 34:
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
	goto tr1;
case 128:
	goto tr1;
case 35:
	if ( (*p) == 97 )
		goto tr62;
	goto tr1;
case 36:
	if ( (*p) == 108 )
		goto tr63;
	goto tr1;
case 37:
	if ( (*p) == 115 )
		goto tr64;
	goto tr1;
case 38:
	if ( (*p) == 101 )
		goto tr65;
	goto tr1;
case 39:
	if ( (*p) == 117 )
		goto tr66;
	goto tr1;
case 40:
	if ( (*p) == 108 )
		goto tr67;
	goto tr1;
case 41:
	if ( (*p) == 108 )
		goto tr52;
	goto tr1;
case 42:
	if ( (*p) == 114 )
		goto tr68;
	goto tr1;
case 43:
	if ( (*p) == 117 )
		goto tr69;
	goto tr1;
case 44:
	if ( (*p) == 101 )
		goto tr70;
	goto tr1;
case 45:
	switch( (*p) ) {
		case 98: goto tr71;
		case 102: goto tr72;
		case 110: goto tr73;
		case 114: goto tr74;
		case 116: goto tr75;
		case 117: goto tr76;
	}
	goto tr49;
case 46:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr77;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr77;
	} else
		goto tr77;
	goto tr1;
case 47:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr78;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr78;
	} else
		goto tr78;
	goto tr1;
case 48:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr79;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr79;
	} else
		goto tr79;
	goto tr1;
case 49:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr80;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr80;
	} else
		goto tr80;
	goto tr1;
case 50:
	switch( (*p) ) {
		case 34: goto tr82;
		case 92: goto tr83;
	}
	goto tr81;
case 51:
	if ( (*p) == 123 )
		goto tr84;
	goto tr1;
case 52:
	switch( (*p) ) {
		case 32: goto tr84;
		case 34: goto tr85;
		case 125: goto tr86;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr84;
	goto tr1;
case 53:
	switch( (*p) ) {
		case 34: goto tr88;
		case 92: goto tr89;
	}
	goto tr87;
case 54:
	switch( (*p) ) {
		case 32: goto tr90;
		case 58: goto tr91;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr90;
	goto tr1;
case 55:
	switch( (*p) ) {
		case 32: goto tr91;
		case 34: goto tr92;
		case 45: goto tr93;
		case 91: goto tr95;
		case 102: goto tr96;
		case 110: goto tr97;
		case 116: goto tr98;
		case 123: goto tr99;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr94;
	} else if ( (*p) >= 9 )
		goto tr91;
	goto tr1;
case 56:
	switch( (*p) ) {
		case 34: goto tr101;
		case 92: goto tr102;
	}
	goto tr100;
case 57:
	switch( (*p) ) {
		case 32: goto tr103;
		case 44: goto tr104;
		case 125: goto tr105;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr103;
	goto tr1;
case 58:
	switch( (*p) ) {
		case 32: goto tr106;
		case 34: goto tr85;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr106;
	goto tr1;
case 59:
	switch( (*p) ) {
		case 32: goto tr107;
		case 34: goto tr85;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr107;
	goto tr1;
case 60:
	switch( (*p) ) {
		case 32: goto tr107;
		case 34: goto tr108;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr107;
	goto tr1;
case 61:
	switch( (*p) ) {
		case 34: goto tr110;
		case 92: goto tr111;
	}
	goto tr109;
case 62:
	switch( (*p) ) {
		case 32: goto tr112;
		case 58: goto tr113;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr112;
	goto tr1;
case 63:
	switch( (*p) ) {
		case 32: goto tr113;
		case 34: goto tr114;
		case 45: goto tr115;
		case 91: goto tr117;
		case 102: goto tr118;
		case 110: goto tr119;
		case 116: goto tr120;
		case 123: goto tr121;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr116;
	} else if ( (*p) >= 9 )
		goto tr113;
	goto tr1;
case 64:
	switch( (*p) ) {
		case 34: goto tr123;
		case 92: goto tr124;
	}
	goto tr122;
case 65:
	switch( (*p) ) {
		case 32: goto tr125;
		case 125: goto tr105;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr125;
	goto tr1;
case 129:
	if ( (*p) == 32 )
		goto tr105;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr105;
	goto tr1;
case 66:
	switch( (*p) ) {
		case 98: goto tr126;
		case 102: goto tr127;
		case 110: goto tr128;
		case 114: goto tr129;
		case 116: goto tr130;
		case 117: goto tr131;
	}
	goto tr122;
case 67:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr132;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr132;
	} else
		goto tr132;
	goto tr1;
case 68:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr133;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr133;
	} else
		goto tr133;
	goto tr1;
case 69:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr134;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr134;
	} else
		goto tr134;
	goto tr1;
case 70:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr135;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr135;
	} else
		goto tr135;
	goto tr1;
case 71:
	switch( (*p) ) {
		case 34: goto tr137;
		case 92: goto tr138;
	}
	goto tr136;
case 72:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr116;
	goto tr1;
case 73:
	switch( (*p) ) {
		case 32: goto tr139;
		case 46: goto tr140;
		case 69: goto tr141;
		case 101: goto tr141;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr116;
	} else if ( (*p) >= 9 )
		goto tr139;
	goto tr1;
case 74:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr143;
	goto tr1;
case 75:
	switch( (*p) ) {
		case 32: goto tr139;
		case 69: goto tr141;
		case 101: goto tr141;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr143;
	} else if ( (*p) >= 9 )
		goto tr139;
	goto tr1;
case 76:
	switch( (*p) ) {
		case 43: goto tr144;
		case 45: goto tr144;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr145;
	goto tr1;
case 77:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr145;
	goto tr1;
case 78:
	switch( (*p) ) {
		case 32: goto tr139;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr145;
	} else if ( (*p) >= 9 )
		goto tr139;
	goto tr1;
case 79:
	if ( (*p) == 97 )
		goto tr146;
	goto tr1;
case 80:
	if ( (*p) == 108 )
		goto tr147;
	goto tr1;
case 81:
	if ( (*p) == 115 )
		goto tr148;
	goto tr1;
case 82:
	if ( (*p) == 101 )
		goto tr149;
	goto tr1;
case 83:
	if ( (*p) == 117 )
		goto tr150;
	goto tr1;
case 84:
	if ( (*p) == 108 )
		goto tr151;
	goto tr1;
case 85:
	if ( (*p) == 108 )
		goto tr125;
	goto tr1;
case 86:
	if ( (*p) == 114 )
		goto tr152;
	goto tr1;
case 87:
	if ( (*p) == 117 )
		goto tr153;
	goto tr1;
case 88:
	if ( (*p) == 101 )
		goto tr154;
	goto tr1;
case 89:
	switch( (*p) ) {
		case 98: goto tr155;
		case 102: goto tr156;
		case 110: goto tr157;
		case 114: goto tr158;
		case 116: goto tr159;
		case 117: goto tr160;
	}
	goto tr109;
case 90:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr161;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr161;
	} else
		goto tr161;
	goto tr1;
case 91:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr162;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr162;
	} else
		goto tr162;
	goto tr1;
case 92:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr163;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr163;
	} else
		goto tr163;
	goto tr1;
case 93:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr164;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr164;
	} else
		goto tr164;
	goto tr1;
case 94:
	switch( (*p) ) {
		case 34: goto tr166;
		case 92: goto tr167;
	}
	goto tr165;
case 95:
	switch( (*p) ) {
		case 98: goto tr168;
		case 102: goto tr169;
		case 110: goto tr170;
		case 114: goto tr171;
		case 116: goto tr172;
		case 117: goto tr173;
	}
	goto tr100;
case 96:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr174;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr174;
	} else
		goto tr174;
	goto tr1;
case 97:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr175;
	} else
		goto tr175;
	goto tr1;
case 98:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr176;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr176;
	} else
		goto tr176;
	goto tr1;
case 99:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr177;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr177;
	} else
		goto tr177;
	goto tr1;
case 100:
	switch( (*p) ) {
		case 34: goto tr179;
		case 92: goto tr180;
	}
	goto tr178;
case 101:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr94;
	goto tr1;
case 102:
	switch( (*p) ) {
		case 32: goto tr181;
		case 44: goto tr182;
		case 46: goto tr183;
		case 69: goto tr184;
		case 101: goto tr184;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr94;
	} else if ( (*p) >= 9 )
		goto tr181;
	goto tr1;
case 103:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr185;
	goto tr1;
case 104:
	switch( (*p) ) {
		case 32: goto tr181;
		case 44: goto tr182;
		case 69: goto tr184;
		case 101: goto tr184;
		case 125: goto tr142;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr185;
	} else if ( (*p) >= 9 )
		goto tr181;
	goto tr1;
case 105:
	switch( (*p) ) {
		case 43: goto tr186;
		case 45: goto tr186;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr187;
	goto tr1;
case 106:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr187;
	goto tr1;
case 107:
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
	goto tr1;
case 108:
	if ( (*p) == 97 )
		goto tr188;
	goto tr1;
case 109:
	if ( (*p) == 108 )
		goto tr189;
	goto tr1;
case 110:
	if ( (*p) == 115 )
		goto tr190;
	goto tr1;
case 111:
	if ( (*p) == 101 )
		goto tr191;
	goto tr1;
case 112:
	if ( (*p) == 117 )
		goto tr192;
	goto tr1;
case 113:
	if ( (*p) == 108 )
		goto tr193;
	goto tr1;
case 114:
	if ( (*p) == 108 )
		goto tr103;
	goto tr1;
case 115:
	if ( (*p) == 114 )
		goto tr194;
	goto tr1;
case 116:
	if ( (*p) == 117 )
		goto tr195;
	goto tr1;
case 117:
	if ( (*p) == 101 )
		goto tr196;
	goto tr1;
case 118:
	switch( (*p) ) {
		case 98: goto tr197;
		case 102: goto tr198;
		case 110: goto tr199;
		case 114: goto tr200;
		case 116: goto tr201;
		case 117: goto tr202;
	}
	goto tr87;
case 119:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr203;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr203;
	} else
		goto tr203;
	goto tr1;
case 120:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr204;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr204;
	} else
		goto tr204;
	goto tr1;
case 121:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr205;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr205;
	} else
		goto tr205;
	goto tr1;
case 122:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr206;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr206;
	} else
		goto tr206;
	goto tr1;
case 123:
	switch( (*p) ) {
		case 34: goto tr208;
		case 92: goto tr209;
	}
	goto tr207;
case 130:
	goto tr1;
	}

	tr1: cs = 0; goto _again;
	tr0: cs = 1; goto _again;
	tr2: cs = 2; goto _again;
	tr10: cs = 2; goto f4;
	tr13: cs = 2; goto f6;
	tr14: cs = 2; goto f7;
	tr15: cs = 2; goto f8;
	tr16: cs = 2; goto f9;
	tr17: cs = 2; goto f10;
	tr23: cs = 2; goto f12;
	tr12: cs = 3; goto _again;
	tr25: cs = 3; goto f14;
	tr18: cs = 4; goto _again;
	tr19: cs = 5; goto f11;
	tr20: cs = 6; goto f11;
	tr21: cs = 7; goto f11;
	tr22: cs = 8; goto f11;
	tr3: cs = 9; goto f0;
	tr211: cs = 10; goto _again;
	tr212: cs = 11; goto _again;
	tr27: cs = 12; goto f16;
	tr6: cs = 13; goto _again;
	tr29: cs = 14; goto _again;
	tr30: cs = 15; goto _again;
	tr31: cs = 16; goto _again;
	tr7: cs = 17; goto _again;
	tr33: cs = 18; goto _again;
	tr34: cs = 19; goto _again;
	tr8: cs = 20; goto _again;
	tr36: cs = 21; goto _again;
	tr37: cs = 22; goto _again;
	tr39: cs = 24; goto _again;
	tr40: cs = 25; goto _again;
	tr49: cs = 25; goto f4;
	tr71: cs = 25; goto f6;
	tr72: cs = 25; goto f7;
	tr73: cs = 25; goto f8;
	tr74: cs = 25; goto f9;
	tr75: cs = 25; goto f10;
	tr81: cs = 25; goto f12;
	tr52: cs = 26; goto _again;
	tr43: cs = 26; goto f2;
	tr48: cs = 26; goto f3;
	tr50: cs = 26; goto f5;
	tr82: cs = 26; goto f13;
	tr65: cs = 26; goto f18;
	tr70: cs = 26; goto f19;
	tr54: cs = 26; goto f20;
	tr53: cs = 27; goto _again;
	tr55: cs = 27; goto f20;
	tr41: cs = 28; goto f0;
	tr42: cs = 29; goto f1;
	tr56: cs = 30; goto _again;
	tr59: cs = 31; goto f15;
	tr57: cs = 32; goto _again;
	tr60: cs = 33; goto f16;
	tr61: cs = 34; goto f17;
	tr45: cs = 35; goto _again;
	tr62: cs = 36; goto _again;
	tr63: cs = 37; goto _again;
	tr64: cs = 38; goto _again;
	tr46: cs = 39; goto _again;
	tr66: cs = 40; goto _again;
	tr67: cs = 41; goto _again;
	tr47: cs = 42; goto _again;
	tr68: cs = 43; goto _again;
	tr69: cs = 44; goto _again;
	tr51: cs = 45; goto _again;
	tr83: cs = 45; goto f14;
	tr76: cs = 46; goto _again;
	tr77: cs = 47; goto f11;
	tr78: cs = 48; goto f11;
	tr79: cs = 49; goto f11;
	tr80: cs = 50; goto f11;
	tr84: cs = 52; goto _again;
	tr85: cs = 53; goto _again;
	tr87: cs = 53; goto f4;
	tr197: cs = 53; goto f6;
	tr198: cs = 53; goto f7;
	tr199: cs = 53; goto f8;
	tr200: cs = 53; goto f9;
	tr201: cs = 53; goto f10;
	tr207: cs = 53; goto f12;
	tr90: cs = 54; goto _again;
	tr88: cs = 54; goto f21;
	tr208: cs = 54; goto f22;
	tr91: cs = 55; goto _again;
	tr92: cs = 56; goto _again;
	tr100: cs = 56; goto f4;
	tr168: cs = 56; goto f6;
	tr169: cs = 56; goto f7;
	tr170: cs = 56; goto f8;
	tr171: cs = 56; goto f9;
	tr172: cs = 56; goto f10;
	tr178: cs = 56; goto f12;
	tr103: cs = 57; goto _again;
	tr95: cs = 57; goto f2;
	tr99: cs = 57; goto f3;
	tr101: cs = 57; goto f5;
	tr179: cs = 57; goto f13;
	tr191: cs = 57; goto f18;
	tr196: cs = 57; goto f19;
	tr181: cs = 57; goto f20;
	tr104: cs = 58; goto _again;
	tr182: cs = 58; goto f20;
	tr106: cs = 59; goto _again;
	tr107: cs = 60; goto _again;
	tr108: cs = 61; goto _again;
	tr109: cs = 61; goto f4;
	tr155: cs = 61; goto f6;
	tr156: cs = 61; goto f7;
	tr157: cs = 61; goto f8;
	tr158: cs = 61; goto f9;
	tr159: cs = 61; goto f10;
	tr165: cs = 61; goto f12;
	tr112: cs = 62; goto _again;
	tr110: cs = 62; goto f21;
	tr166: cs = 62; goto f22;
	tr113: cs = 63; goto _again;
	tr114: cs = 64; goto _again;
	tr122: cs = 64; goto f4;
	tr126: cs = 64; goto f6;
	tr127: cs = 64; goto f7;
	tr128: cs = 64; goto f8;
	tr129: cs = 64; goto f9;
	tr130: cs = 64; goto f10;
	tr136: cs = 64; goto f12;
	tr125: cs = 65; goto _again;
	tr117: cs = 65; goto f2;
	tr121: cs = 65; goto f3;
	tr123: cs = 65; goto f5;
	tr137: cs = 65; goto f13;
	tr149: cs = 65; goto f18;
	tr154: cs = 65; goto f19;
	tr139: cs = 65; goto f20;
	tr124: cs = 66; goto _again;
	tr138: cs = 66; goto f14;
	tr131: cs = 67; goto _again;
	tr132: cs = 68; goto f11;
	tr133: cs = 69; goto f11;
	tr134: cs = 70; goto f11;
	tr135: cs = 71; goto f11;
	tr115: cs = 72; goto f0;
	tr116: cs = 73; goto f1;
	tr140: cs = 74; goto _again;
	tr143: cs = 75; goto f15;
	tr141: cs = 76; goto _again;
	tr144: cs = 77; goto f16;
	tr145: cs = 78; goto f17;
	tr118: cs = 79; goto _again;
	tr146: cs = 80; goto _again;
	tr147: cs = 81; goto _again;
	tr148: cs = 82; goto _again;
	tr119: cs = 83; goto _again;
	tr150: cs = 84; goto _again;
	tr151: cs = 85; goto _again;
	tr120: cs = 86; goto _again;
	tr152: cs = 87; goto _again;
	tr153: cs = 88; goto _again;
	tr111: cs = 89; goto _again;
	tr167: cs = 89; goto f14;
	tr160: cs = 90; goto _again;
	tr161: cs = 91; goto f11;
	tr162: cs = 92; goto f11;
	tr163: cs = 93; goto f11;
	tr164: cs = 94; goto f11;
	tr102: cs = 95; goto _again;
	tr180: cs = 95; goto f14;
	tr173: cs = 96; goto _again;
	tr174: cs = 97; goto f11;
	tr175: cs = 98; goto f11;
	tr176: cs = 99; goto f11;
	tr177: cs = 100; goto f11;
	tr93: cs = 101; goto f0;
	tr94: cs = 102; goto f1;
	tr183: cs = 103; goto _again;
	tr185: cs = 104; goto f15;
	tr184: cs = 105; goto _again;
	tr186: cs = 106; goto f16;
	tr187: cs = 107; goto f17;
	tr96: cs = 108; goto _again;
	tr188: cs = 109; goto _again;
	tr189: cs = 110; goto _again;
	tr190: cs = 111; goto _again;
	tr97: cs = 112; goto _again;
	tr192: cs = 113; goto _again;
	tr193: cs = 114; goto _again;
	tr98: cs = 115; goto _again;
	tr194: cs = 116; goto _again;
	tr195: cs = 117; goto _again;
	tr89: cs = 118; goto _again;
	tr209: cs = 118; goto f14;
	tr202: cs = 119; goto _again;
	tr203: cs = 120; goto f11;
	tr204: cs = 121; goto f11;
	tr205: cs = 122; goto f11;
	tr206: cs = 123; goto f11;
	tr35: cs = 124; goto _again;
	tr5: cs = 124; goto f2;
	tr9: cs = 124; goto f3;
	tr11: cs = 124; goto f5;
	tr24: cs = 124; goto f13;
	tr32: cs = 124; goto f18;
	tr38: cs = 124; goto f19;
	tr210: cs = 124; goto f20;
	tr4: cs = 125; goto f1;
	tr26: cs = 126; goto f15;
	tr28: cs = 127; goto f17;
	tr44: cs = 128; goto _again;
	tr58: cs = 128; goto f20;
	tr105: cs = 129; goto _again;
	tr142: cs = 129; goto f20;
	tr86: cs = 130; goto _again;

	f6: _acts = _json_actions + 1; goto execFuncs;
	f7: _acts = _json_actions + 3; goto execFuncs;
	f8: _acts = _json_actions + 5; goto execFuncs;
	f9: _acts = _json_actions + 7; goto execFuncs;
	f10: _acts = _json_actions + 9; goto execFuncs;
	f4: _acts = _json_actions + 11; goto execFuncs;
	f11: _acts = _json_actions + 13; goto execFuncs;
	f14: _acts = _json_actions + 15; goto execFuncs;
	f5: _acts = _json_actions + 17; goto execFuncs;
	f0: _acts = _json_actions + 19; goto execFuncs;
	f1: _acts = _json_actions + 21; goto execFuncs;
	f15: _acts = _json_actions + 23; goto execFuncs;
	f16: _acts = _json_actions + 25; goto execFuncs;
	f17: _acts = _json_actions + 27; goto execFuncs;
	f20: _acts = _json_actions + 29; goto execFuncs;
	f19: _acts = _json_actions + 31; goto execFuncs;
	f18: _acts = _json_actions + 33; goto execFuncs;
	f2: _acts = _json_actions + 35; goto execFuncs;
	f3: _acts = _json_actions + 37; goto execFuncs;
	f21: _acts = _json_actions + 39; goto execFuncs;
	f12: _acts = _json_actions + 41; goto execFuncs;
	f13: _acts = _json_actions + 44; goto execFuncs;
	f22: _acts = _json_actions + 47; goto execFuncs;

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
	case 9:
#line 7 "/home/matiu/projects/yajp/number.rl"
	{
        #ifdef DEBUG
        std::cout << "setNegative" << std::endl;
        #endif
        intIsNeg = true;
    }
	break;
	case 10:
#line 13 "/home/matiu/projects/yajp/number.rl"
	{
        intPart *= 10;
        intPart += *p - '0';
        #ifdef DEBUG
        std::cout << "recordInt " << *p << " - " << intPart << std::endl; 
        #endif
    }
	break;
	case 11:
#line 20 "/home/matiu/projects/yajp/number.rl"
	{
        intPart *= 10;
        intPart += *p - '0';
        --expPart1; // the 'actual' end exponent will be way at the end
        #ifdef DEBUG
        std::cout << "recordDecimal " << *p << " - " << intPart << " - " << expPart1 << std::endl; 
        #endif
    }
	break;
	case 12:
#line 28 "/home/matiu/projects/yajp/number.rl"
	{
        if (*p == '-')
            expIsNeg = true;
        #ifdef DEBUG
        std::cout << "setExpNeg " << expIsNeg << std::endl;
        #endif
    }
	break;
	case 13:
#line 35 "/home/matiu/projects/yajp/number.rl"
	{
        expPart2 *= 10;
        expPart2 += *p - '0';
        #ifdef DEBUG
        std::cout << "recordExponent " << *p << " - " << expPart2 << std::endl;
        #endif
    }
	break;
	case 14:
#line 42 "/home/matiu/projects/yajp/number.rl"
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
            callback.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                callback.foundSimpleValue(result2);
            } else {
                callback.foundSimpleValue(result);
            }
        }
    }
	break;
	case 15:
#line 22 "/home/matiu/projects/yajp/json.rl"
	{ callback.foundSimpleValue(true); }
	break;
	case 16:
#line 23 "/home/matiu/projects/yajp/json.rl"
	{ callback.foundSimpleValue(false); }
	break;
	case 17:
#line 25 "/home/matiu/projects/yajp/json.rl"
	{
        // Called when we hit the start of an array
        // The Array is recursive, as it can hold further arrays, so we need to employ our fake stack and use fcall
        p--;
        callback.startArray();
        {stack[top++] = cs; cs = 23; goto _again;}
        callback.endArray();
    }
	break;
	case 18:
#line 33 "/home/matiu/projects/yajp/json.rl"
	{
        // Called when we hit the start of an object '{' starts recursing to get the inside parts of an object
        // The Object is recursive, as it can hold further objects, so we need to employ our fake stack and use fcall
        p--;
        callback.startObj();
        {stack[top++] = cs; cs = 51; goto _again;}
        callback.endObj();
    }
	break;
	case 19:
#line 41 "/home/matiu/projects/yajp/json.rl"
	{
        // Called when we found a new property for a value
        callback.propertyName(std::move(currentString));
    }
	break;
#line 1481 "/home/matiu/projects/yajp/json.hpp"
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
	case 14:
#line 42 "/home/matiu/projects/yajp/number.rl"
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
            callback.foundSimpleValue(result);
        } else {
            unsigned long result = intPart;
            while (expPart-- > 0)
                result *= 10;
            if (intIsNeg) {
                signed long result2 = -result;
                callback.foundSimpleValue(result2);
            } else {
                callback.foundSimpleValue(result);
            }
        }
    }
	break;
#line 1525 "/home/matiu/projects/yajp/json.hpp"
		}
	}
	}

	_out: {}
	}

#line 87 "/home/matiu/projects/yajp/json.rl"
}

} // namespace yajp

#endif // YAJP_JSON_HPP
