%%{
    machine string;

    action recordBackspace { output += '\b'; }
    action recordFormFeed { output += '\f'; }
    action recordNewLine { output += '\n'; }
    action recordReturn { output += '\r'; }
    action recordTab { output += '\t'; }
    action getChar { output += *p; }
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
            output += static_cast<unsigned char>(uniChar);
        } else if (uniChar <= 0x7ff) {
            output += (uniChar >> 6) | 0xC0; // 110 to indicate 2 byte encoding + 5 bits of data
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0xffff) {
            output += (uniChar >> 12) | 0xE0; // 1110 to indicate 3 byte encoding + 4 bits of data
            output += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        } else if (uniChar <= 0x10ffff) {
            output += (uniChar >> 18) | 0xF0; // 11110 to indicate 4 byte encoding + 3 bits of data
            output += ((uniChar >> 12) & 0x3f) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += ((uniChar >> 6) & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
            output += (uniChar & 0x3F) | 0x80; // 10 to indicate a byte in the sequence + 6 bits of data 
        }
        // TODO: Handle unicode numbers with more than 3 digits
    }
    action gotString {
        ++p;
        return output;
    }

    esc_b = "\\b"@recordBackspace;
    esc_f = "\\f"@recordFormFeed;
    esc_n = "\\n"@recordNewLine;
    esc_r = "\\r"@recordReturn;
    esc_t = "\\t"@recordTab;
    normal_char = [^\\"]@getChar;
    hex_digit = [0-9a-fA-F]@getUnicode;
    esc_uni = '\\u'.hex_digit+ % endUnicode;
    esc_any = "\\".[^bfnrut]@getChar;

    string = (esc_b|esc_f|esc_n|esc_r|esc_t|esc_any|esc_uni|normal_char)**:>'"' @ gotString; 
}%%
