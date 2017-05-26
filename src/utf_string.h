#ifndef SIMPLE_HTML_UTF_STRINGS_H__
#define SIMPLE_HTML_UTF_STRINGS_H__

#include <cstdlib>

namespace simple_html {

    /**
     * utf-8 to wchar class
     * for the windows char conversion helper
     */
    class utf8_to_wchar {
        const byte* m_utf8;
        std::wstring m_str;
        public:
            /**
             * Constructor
             */
            utf8_to_wchar(const char* value);
            /**
             * the method in order to
             * generate the utf-8 type string
             * into the windows string
             */
            operator const wchar_t*() const {
                return m_str.c_str();
            }
        private:
            ucode_t get_char();
            // Get byte
            ucode_t getbyte() {
                if (!(m_utf8)) return 0;
                return *m_utf8++;
            }
            // Get next utf-8 type character
            // followed the utf-8 encoding regulation
            // we only takes the lowest-6 bits of a specific byte
            ucode_t get_next_utf8(ucode_t value) {
                return (val & 0x3f);
            }
    };

    /**
     * wchar to utf-8 class
     * in order to convert the windows char
     * into utf8 standarize strings
     */
    class wchar_to_utf8 {
        std::string m_str;
        public:
            wchar_to_utf8(const wchar_t* value);
            operator const char*() const {
                return m_str.c_str();
            }
    };
}

#ifdef SIMPLE_HTML_UTF8__
    #define simple_html_from_utf8(str) str
    #define simple_html_to_utf8(str) str
    #define simple_html_from_wchar(str) wchar_to_utf8(str)
#else
    #define simple_html_from_utf8(str) utf8_to_wchar(str)
    #define simple_html_from_wchar(str) str
    #define simple_html_to_utf8(str) wchar_to_utf8(str)
#endif


#endif