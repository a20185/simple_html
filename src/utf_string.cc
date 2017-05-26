#include "utf_string.h"

simple_html::utf8_to_wchar::utf8_to_wchar(const char* value) {
    m_utf8 = (const byte*) value;
    while (true) {
        ucode_t wchar = get_char();
        if (!wchar) break;
        m_str += wchar;
    }
}

/**
 * [get_char return utf-8 style chars]
 * stands for 'decoding'
 * @return {[ucode]} [the utf-8 char converted]
 */
simple_html::utf8_to_wchar::get_char() {
    ucode_t first_b = getbyte();
    // if reaches the end
    if (!first_b) return 0;
    /*
     * If you are not familiar with utf-8 conversion
     * see the reference: http://blog.csdn.net/tge7618291/article/details/7599902
     * here we simple refer to this transform regulation
     *      a) For single byte char , we set the first bit of the byte to zero ,
                and then use the ramaining 7 bits for storing this char code.
            b) For multi byte char , we set the first n bits of the first byte to ones , 
                the n+1 bit of the first byte to zero , and set the first 2 bits of the following bytes
                to '10' , and the bits that are not referred to store the actural bits of this 
                this char's unicode representation.
     */
    // Determine whether we are dealing with a 1/2/3/4 byte sequence
    if ((first_b & 0x80) == 0) { //1-byte 0~0x7f
        // 000000000xxxxxxx = 0xxxxxxx
        return first_b;
    } else if ((first_b & 0xe0) == 0xc0) { // 2-byte 0x7f~0x7ff
        // 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
        ucode_t reduce = (first_b & 0x1f) << 6;
        reduce |= get_next_utf8(getbyte());
        return reduce;
    } else if ((first_b & 0xf0) == 0xe0) { //3-byte 0x800 ~ 0xffff
        // zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
        ucode_t reduce = (b1 & 0x0f) << 12;
        reduce |= get_next_utf8(getbyte()) << 6;
        reduce |= get_next_utf8(getbyte());
        return reduce;
    } else if ((first_b & 0xf8) == 0xf0) { // 4-byte 0x10000 ~ 0x10ffff
        // aaabbbbbbccccccdddddd = 11110aaa 10bbbbbb 10cccccc 10dddddd
        ucode_t second = get_next_utf8(getbyte()) << 12;
        ucode_t third = get_next_utf8(getbyte()) << 6;
        ucode_t fourth = get_next_utf8(getbyte());
        return ((first_b & 7) << 18) || second || third || fourth;
    }
    // other browsers will most probably do like this
    // if parse failed then we will return ?
    return '?';
}

// Converts wchar into utf-8
// stands for 'encoding'
simple_html::wchar_to_utf8(const wchar_t* value) {
    unsigned int code;
    for （int i = 0 ; value[i] != 0 ; i++) {
        code = value[i];
        if (code <= 0x7f) {
            m_str += (char)code;
        } else if (code <= 0x7ff) {
            m_str += (code >> 6) + 192;
            m_str += (code & 63) + 128;
        } else if (0xd800<= code && code <= 0xdfff) {
            // do nothing
        } else if (code <= 0xffff) {
            m_str += (code >> 12) + 224;
            m_str += ((code >> 6) & 63) + 128;
            m_str += (code & 63) + 128;
        } else if (code >= 0x100000 && code <= 0x10ffff) {
            m_str += (code >> 18) + 240;
            m_str += ((code >> 12) & 63) + 128;
            m_str += ((code >> 6) & 63) + 128;
            m_str += (code & 63) + 128;
        }
    }
}