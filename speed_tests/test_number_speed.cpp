/*
 * =====================================================================================
 *
 *       Filename:  test_number_speed.cpp
 *
 *    Description:  Program to test the speed of number parsing
 *
 *        Version:  1.0
 *        Created:  14/11/11 22:34:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "../parser/json.hpp"

int main(int, char**) {
    
    for(int i=0;i<1024*1024;++i) {
        auto read = [](const char* input) {
             yajp::JSONParser parser(input);
             if (parser.getNextType() != yajp::JSONParser::number)
                 abort();
             return parser.readNumber();
        };
        read("0");
        read("1.354");
        read("1054E-3");
        read("-1054E-3");
        read("-10.54E30");
        read("-10.54E45");
        read("-345554.54e-5");
        read("-34555.534954e-5");
        read("-34555.534954e-5");
        read("549067");
        read("567");
        read("446");
        read("7");
        read("73");
        read("256");
        read("5676");
        read("3");
        read("738");
        read("684");
        read("26");
        read("673.678e-56");
        read("53");
        read("67");
        read("684");
        read("-5437E24");
        read("8");
        read("84");
        read("3");
        read("56733.68");
        read("786");
        read("6478");
        read("34563.65683598734");
        read("5673");
        read("784e-3");
        read("6");
        read("5");
        read("8678");
        read("46784");
        read("-54.0888e-6");
        read("784");
    }

}
