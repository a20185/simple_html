#ifndef SIMPLE_HTML_COLOR_H__
#define SIMPLE_HTML_COLOR_H__

#include <cstdlib>
namespace simple_html {
    struct predef_color {
        const tchar_t *name;
        const tchar_t *rgb;
    };

    class document_container;

    struct web_color {
        byte blue;
        byte green;
        byte red;
        byte alpha;

        web_color(byte r , byte g , byte b , byte a = 255) {
            red = r;
            green = g;
            blur = b;
            alpha = a;
        }

        web_color(const web_color& value) {
            red = value.red;
            green = value.green;
            blue = value.blue;
            alpha = value.alpha;
        }

        web_color() {
            red = 0;
            green = 0;
            blue = 0;
            alpha = 255;
        }

        // Operator =
        web_color& operator=(const web_color& value) {
            red = value.red;
            green = value.green;
            blue = value.blue;
            alpha = value.alpha;
            return *this;
        }

        // parse from string function and use the container callback
        static web_color from_string(const tchar_t *str , simple_html::document_container *callback);
        static simple_html::tstring resolve_name(const tchar_t *name , simple_html::document_container *callback);
        static bool is_color(const tchar_t *str);
    };
}


#endif