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
#include "parser/json.hpp"
#include <assert.h>

int main(int argc, char** argv) {
    if (argc > 1)  {
        std::string type = "d"; // double
        std::string input = argv[1];
        if (argc == 3) {
            type = argv[1];
            input = argv[2];
        }
        yajp::JSONParser parser(input);
        auto nextType = parser.getNextType(true);
        switch (nextType) {
            case yajp::JSONParser::JSONType::null :
                std::cout << "That's not a number, that's a 'null'" << std::endl;
                return 1;
            case yajp::JSONParser::JSONType::boolean :
                std::cout << "That's not a number, that's a 'boolean'" << std::endl;
                return 1;
            case yajp::JSONParser::JSONType::array :
                std::cout << "That's not a number, that's a 'array'" << std::endl;
                return 1;
            case yajp::JSONParser::JSONType::object :
                std::cout << "That's not a number, that's a 'object'" << std::endl;
                return 1;
            case yajp::JSONParser::JSONType::number :
                break;
            case yajp::JSONParser::JSONType::string :
                std::cout << "That's not a number, that's a 'string'" << std::endl;
                return 1;
            case yajp::JSONParser::JSONType::HIT_END :
                std::cout << "I hit the end of the input unexpectedly, no JSON found" << std::endl;
                return 1;
            case yajp::JSONParser::JSONType::ERROR :
                std::cout << "That doesn't look like valid json to me" << std::endl;
                return 1;
        }
        if (type == "ul")
            std::cout << "Found Number: " << parser.readNumber<unsigned long>() << std::endl;
        else if (type == "l")
            std::cout << "Found Number: " << parser.readNumber<long>() << std::endl;
        else if (type == "ll")
            std::cout << "Found Number: " << parser.readNumber<long long>() << std::endl;
        else if (type == "ull")
            std::cout << "Found Number: " << parser.readNumber<unsigned long long>() << std::endl;
        else if (type == "i")
            std::cout << "Found Number: " << parser.readNumber<int>() << std::endl;
        else if (type == "c")
            std::cout << "Found Number: " << static_cast<int>(parser.readNumber<char>()) << std::endl;
        else if (type == "uc")
            std::cout << "Found Number: " << static_cast<unsigned int>(parser.readNumber<unsigned char>()) << std::endl;
        else
            std::cout << "Found Number: " << parser.readNumber() << std::endl; // Default to double
    }
    return 0;
}
