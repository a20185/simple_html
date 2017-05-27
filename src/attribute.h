#ifndef SIMPLE_HTML_ATTRIBUTES_H__
#define SIMPLE_HTML_ATTRIBUTES_H__

namespace simple_html {
    struct color_attr {
        // Define the channels of the color
        unsigned char rgb_red;
        unsigned char rgb_green;
        unsigned char rgb_blue;
        unsigned char rgb_alpha;

        // Base Constructor:
        color_attr() {
            rgb_red = 0;
            rgb_green = 0;
            rgb_blue = 0;
            rgb_alpha = 0;
        }

        // Default constructor
        color_attr(unsigned char r , unsigned char g , unsigned char b , unsigned char a = 255) {
            rgb_alpha = a;
            rgb_blue = b;
            rgb_green = g;
            rgb_red = r;
        }
    };

    struct border_attr {
        // style_border border;
        int width;
        color_attr color;

        border_attr() {
            // border = NULL;
            width = 0;
        }
    };
}


#endif