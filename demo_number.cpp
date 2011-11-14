/*
 * =====================================================================================
 *
 *       Filename:  test_number.cpp
 *
 *    Description:  A program for testing the parsing of a JSON number
 *
 *        Version:  1.0
 *        Created:  11/11/11 10:57:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include "number.hpp"
#include <assert.h>

struct CallbackHandler {
    enum LastCall {ul, l, d} lc; // just used to know which one was called
    void foundNumber(unsigned long num) {
        std::cout << "Found unsigned long: " << num << std::endl;
    }
    void foundNumber(long num) {
        std::cout << "Found long: " << num << std::endl;
    }
    void foundNumber(double num) {
        std::cout << "Found double: " << num << std::endl;
    }
};

int main(int argc, char** argv) {
    if (argc > 1)  {
        std::string simpleInt = argv[1];
        CallbackHandler cb;
        yajp::parseNumber(simpleInt, cb);
        assert(cb.lc == CallbackHandler::ul);
    }
    return 0;
}
