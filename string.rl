/** Ragel file for parsing a string in json **/

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

namespace yajp {

%%{
    machine string;

    action recordBackspace { result += '\b'; }
    action recordFormFeed { result += '\f'; }
    action recordNewLine { result += '\n'; }
    action recordReturn { result += '\r'; }
    action recordTab { result += '\t'; }
    action getChar { result += *p; }
    action startUnicode { uniChar = 0; }
    action getUnicode {
        uniChar <<= 4;
        char ch = *p;
        if (ch >= 'a') 
            uniChar += ch - 'a' + 0x0A;
        else if (ch >= 'A')
            uniChar += ch - 'A' + 0x0A;
        else
            uniChar += ch - '0';
    }
    action endUnicode {
        // Encode it into utf-8
        if (uniChar <= 0x7f) {
            result += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            result += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        } else if (uniChar <= 0xffff) {
            result += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            result += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        } else if (uniChar <= 0x10ffff) {
            result += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            result += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate 2 byte encoding + 5 bits of data
            result += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
            result += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 3 bits of data 
        }
    }
    action stringDone {
        callback.foundString(std::move(result));
    }

    esc_b = "\\b"@recordBackspace;
    esc_f = "\\f"@recordFormFeed;
    esc_n = "\\n"@recordNewLine;
    esc_r = "\\r"@recordReturn;
    esc_t = "\\t"@recordTab;
    normal_char = [^\\"]@getChar;
    hex_digit = [0-9a-fA-F]@getUnicode;
    esc_uni = '\\u'.hex_digit{4} % endUnicode;
    esc_any = "\\".[^bfnrut]@getChar;

    string = '"'.(esc_b|esc_f|esc_n|esc_r|esc_t|esc_any|esc_uni|normal_char)**:>'"'@stringDone; 
    main := string;
}%%

// data //////////////////////////////////////////
%%write data;

/**
* @brief Parse a json string
*
* @tparam T any class that has  a 'foundString' method that we can call
* @param json the json that we are parsing .. should be already at the start of the string
* @param callback an object that we can call 'foundString(const std::string&) or foundString(std::string&&) (c++11 move) on
* @param expectedSize the expected size of the output string
*/
template <class T>
void parseString(const std::string& json, T& callback, unsigned long expectedSize=256) {
    // Ragel vars
    int cs;
    const char *p = &json.c_str()[0];
    const char *pe = p + json.length();
    // action vars
    unsigned long uniChar = 0;
    std::string result;
    result.reserve(expectedSize);

    %%write init;
    %%write exec;
}

}
