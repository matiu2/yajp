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
        if (ch > 'a') 
            uniChar += ch - 'a' + 0xa;
        else if (ch > 'A')
            uniChar += ch - 'A' + 0xa;
        else
            uniChar += ch - '0';
    }
    action endUnicode {
        result += uniChar;
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
    esc_any = "\\".[^\\"bfnrut]@getChar;

    hex_digit = [0-9a-fA-F]@getUnicode;
    unicode = '\\u'.hex_digit{1,4} % endUnicode;
    #string = ('"').((esc_b)|(esc_f)|(esc_n)|(esc_r)|(esc_t)|(unicode)|(normal_char))**.('"'@stringDone); 
    string = '"'.(esc_b|esc_f|esc_n|esc_r|esc_t|esc_any|unicode|normal_char)**:>>'"'@stringDone; 
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
    //const char *eof = pe;
    // action vars
    //const char* stringStart = p;
    wchar_t uniChar = 0;
    std::string result;
    result.reserve(expectedSize);

    %%write init;
    %%write exec;
}

}
