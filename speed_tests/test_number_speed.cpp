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

#include "../parser/number.hpp"
#include <vector>

struct CallbackHandler {
    std::vector<double> result;
    inline void foundSimpleValue(double num) {
        result.push_back(num);
    }
};

int main(int, char**) {
    CallbackHandler cb;
    
    for(int i=0;i<1024*1024;++i) {
        cb.result.clear();
        cb.result.reserve(39);
        yajp::parseNumber("0", cb);
        yajp::parseNumber("1.354", cb);
        yajp::parseNumber("1054E-3", cb);
        yajp::parseNumber("-1054E-3", cb);
        yajp::parseNumber("-10.54E30", cb);
        yajp::parseNumber("-10.54E45", cb);
        yajp::parseNumber("-345554.54e-5", cb);
        yajp::parseNumber("-34555.534954e-5", cb);
        yajp::parseNumber("-34555.534954e-5", cb);
        yajp::parseNumber("549067", cb);
        yajp::parseNumber("567", cb);
        yajp::parseNumber("446", cb);
        yajp::parseNumber("7", cb);
        yajp::parseNumber("73", cb);
        yajp::parseNumber("256", cb);
        yajp::parseNumber("5676", cb);
        yajp::parseNumber("3", cb);
        yajp::parseNumber("738", cb);
        yajp::parseNumber("684", cb);
        yajp::parseNumber("26", cb);
        yajp::parseNumber("673.678e-56", cb);
        yajp::parseNumber("53", cb);
        yajp::parseNumber("67", cb);
        yajp::parseNumber("684", cb);
        yajp::parseNumber("-5437E24", cb);
        yajp::parseNumber("8", cb);
        yajp::parseNumber("84", cb);
        yajp::parseNumber("3", cb);
        yajp::parseNumber("56733.68", cb);
        yajp::parseNumber("786", cb);
        yajp::parseNumber("6478", cb);
        yajp::parseNumber("34563.65683598734", cb);
        yajp::parseNumber("5673", cb);
        yajp::parseNumber("784e-3", cb);
        yajp::parseNumber("6", cb);
        yajp::parseNumber("5", cb);
        yajp::parseNumber("8678", cb);
        yajp::parseNumber("46784", cb);
        yajp::parseNumber("-54.0888e-6", cb);
        yajp::parseNumber("784", cb);
    }

}
