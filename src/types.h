#ifndef SIMPLE_HTML_TYPES
#define SIMPLE_HTML_TYPES

#include <cstdlib>
#include <memory>
#include <map>
#include <vector>

namespace simple_html{
    // Definition of Document Class
    class Document;
    // Definition of Element Class
    class Element;

    // Type definition for shorter codes
    typedef std::map<simple_html::tstring , simple_html::tstring> string_map;
    typedef std::vector<std::shared_ptr<simple_html::Element> > element_vector;
    typedef std::vector<int> int_vector;
    typedef std::vector<simple_html::tstring> string_vector;

    // Basic constants for default font decorations
    const unsigned int font_decoration_none = 0x00;
    const unsigned int font_decoration_underline = 0x01;
    const unsigned int font_decoration_linethrough = 0x02;
    const unsigned int font_decoration_overline = 0x04;

    // define for type alias
    typedef unsigned char byte;
    typedef unsigned int ucode_t;

    // Margin Structure :
    //      Stores the margin amount of the css border box;
    //      including left , right , top and bottom amounts;
    //      Simple for usage
    //      .
    struct margins {
        // In webkit and other modern browsers,
        // They support different scales of the units
        // and the margin amount is covered in float values.
        int left;
        int right;
        int top;
        int bottom;
        // Base constructor
        margins() {
            left = 0;
            right = 0;
            top = 0;
            bottom = 0;
        }
        // Default constructor
        margins(int _l , int _r = 0 , int _t = 0 , int _b = 0) {
            left = _l;
            right = _r;
            top = _t;
            bottom = _b;
        }
        int width() const {
            return left + right;
        }
        int height() const {
            return top + bottom;
        }
    };

    // Size structure:
    //      For the support of the reality sizetype and the "size"-releted attributes 
    //      Again in webkit-based browsers and most modern browsers , they support float
    //      Attribute values and different scale units.
    //      .
    struct size {
        int width;
        int height;
        // Base constructor
        size() {
            width = 0;
            height = 0;
        }
        // Default Constructor
        size(int _w , int _h = 0) {
            width = _w;
            height = _h;
        }
    };

    // Position Structure
    //      For support of the "position" attribute of the box element / box object
    //      Here we realize the absolute positioning
    //      Again referenced to the webkit designed
    //      /
    struct position {
        int x;
        int y;
        int height;
        int width;
        // Base Constructor
        position() {
            x = 0;
            y = 0;
            height = 0;
            width = 0;
        }
        // Default constructor
        position(int _x , int _y , int _w = 0 , int _h = 0) {
            x = _x;
            y = _y;
            width = _w;
            height = _h;
        }


        // position getters
        int left() const {
            return x;
        }
        int right() const {
            return x + width;
        }
        int top() const {
            return y;
        }
        int bottom() const {
            return y + height;
        }


        // Operators
        void operator+=(const margins &margin) {
            x -= margin.left;
            y -= margin.top;
            width += margin.left + margin.right;
            height += margin.top + margin.bottom;
        }

        void operator-=(const margins &margin) {
            x += margin.left;
            y += margin.top;
            width -= margin.left + margin.right;
            height -= margin.top + margin.bottom;
        }

        // Operator with the size attribute
        void operator=(const size &_size) {
            width = _size.width;
            height = _size.height;
        }

        // Clear and empty this position
        // While reset it to the default condition
        // and set it to the origin
        void reset() {
            x = 0;
            y = 0;
            width = 0;
            height = 0;
        }

        // Moving function
        // Set the coordinate of the position attribute structure
        // to the given value
        // should perform validicity check.
        void move_to(int _x , int _y) {
            x = _x;
            y = _y;
        }

        // empty function
        // Just use for check if this box contains nothing
        bool empty() const {
            if (!width && !height) return true;
            return false;
        }

        // To judge if a given point is inside this box
        bool is_inside(int _x , int _y) const {
            if (_x >= left() && _x <= right() && _y >= top() && _y <= bottom()) return true;
            return false;
        }

        // Judge if 2 boxes have the intersections
        bool does_intersect(const position *value) {
            if (!val) return true;
            return (
               (
                    (left() <= value -> right()) &&
                    (right() >= value -> left()) &&
                    (top() <= value -> bottom()) &&
                    (bottom() >= value -> top())
                ) || (
                    (value -> left() <= right()) &&
                    (value -> right() >= left()) &&
                    (value -> top() <= bottom()) &&
                    (value -> bottom() >= top())
                )
            );
        }
    };


    // Font metrics structure
    //      Used to describe the mesurements of font-related css styles
    struct font_metrics {
        int height;
        int ascent;
        int descent;
        int x_height;
        bool draw_space;

        // Base constructor
        font_metrics() {
            height = 0;
            ascent = 0;
            descent = 0;
            x_height = 0;
            draw_space = true;
        }
        // Get the baseline of the font
        int baseline() {
            return descent;
        }
    };

    struct font_item {
        uint_ptr font;
        font_metrics metrics;
    };
    typedef std::map<string , font_item> font_map;


    enum draw_flag {
        DRAW_ROOT,
        DRAW_BLOCK,
        DRAW_FLOATS,
        DRAW_INLINES,
        DRAW_POSITIONED
    };

    enum style_display {
        DISPLAY_NONE,
        DISPLAY_BLOCK,
        DISPLAY_INLINE,
        DISPLAY_INLINE_BLOCK,
        DISPLAY_INLINE_BLOCK_TABLE,
        DISPLAY_LIST_ITEM,
        DISPLAY_TABLE,
        DISPLAY_TABLE_CAPTION,
        DISPLAY_TABLE_CELL,
        DISPLAY_TABLE_COLUMN,
        DISPLAY_TABLE_COLUMN_GROUP,
        DISPLAY_TABLE_FOOTER_GROUP,
        DISPLAY_TABLE_HEADER_GROUP,
        DISPLAY_TABLE_ROW,
        DISPLAY_TABLE_ROW_GROUP,
        DISPLAY_INLINE_TEXT
    };

    enum style_border {
        BORDER_NOPE,
        BORDER_NONE,
        BORDER_HIDDEN,
        BORDER_DOTTED,
        BORDER_DASHED,
        BORDER_SOLID,
        BORDER_DOUBLE
    };

    enum font_size {
        FONTSIZE_XX_SMALL,
        FONTSIZE_X_SMALL,
        FONTSIZE_SMALL,
        FONTSIZE_MEDIUM,
        FONTSIZE_LARGE,
        FONTSIZE_X_LARGE,
        FONTSIZE_XX_LARGE,
        FONTSIZE_SMALLER,
        FONTSIZE_LARGER
    };

    enum font_style {
        FONT_STYLE_NORMAL,
        FONT_STYLE_ITALIC
    };

    enum font_variant {
        FONT_VARIANT_NORMAL,
        FONT_VARIANT_ITALIC
    };

    enum font_weight {
        FONT_WEIGHT_NORMAL,
        FONT_WEIGHT_BOLD,
        FONT_WEIGHT_BOLDER,
        FONT_WEIGHT_LIGHTER,
        FONT_WEIGHT_100,
        FONT_WEIGHT_200,
        FONT_WEIGHT_300,
        FONT_WEIGHT_400,
        FONT_WEIGHT_500,
        FONT_WEIGHT_600,
        FONT_WEIGHT_700
    };

    enum list_style_type {
        LIST_STYLE_TYPE_NONE,
        LIST_STYLE_TYPE_CIRCLE,
        LIST_STYLE_TYPE_DISC,
        LIST_STYLE_TYPE_SQUARE,
        LIST_STYLE_TYPE_ARMENIAN,
        LIST_STYLE_TYPE_CJK_IDEOGRAPHIC,
        LIST_STYLE_TYPE_DECIMAL,
        LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO,
        LIST_STYLE_TYPE_GEORGIAN,
        LIST_STYLE_TYPE_HEBREW,
        LIST_STYLE_TYPE_HIRAGANA,
        LIST_STYLE_TYPE_HIRAGANA_IROHA,
        LIST_STYLE_TYPE_KATAKANA,
        LIST_STYLE_TYPE_KATAKANA_IROHA,
        LIST_STYLE_TYPE_LOWER_ALPHA,
        LIST_STYLE_TYPE_LOWER_GREEK,
        LIST_STYLE_TYPE_LOWER_LATIN,
        LIST_STYLE_TYPE_LOWER_ROMAN,
        LIST_STYLE_TYPE_UPPER_ALPHA,
        LIST_STYLE_TYPE_UPPER_LATIN,
        LIST_STYLE_TYPE_UPPER_ROMAN
    };

    // TODO LISTSTYLE POSITION ENUM
    enum list_style_position {
        LIST_STYLE_POSITION_INSIDE,
        LIST_STYLE_POSITION_OUTSIDE
    };

    // Todo: vertical align enum
    enum vertical_align {
        VERTICAL_ALIGN_BASELINE,
        VERTICAL_ALIGN_SUB,
        VERTICAL_ALIGN_SUPER,
        VERTICAL_ALIGN_TOP,
        VERTICAL_ALIGN_NEXT_TOP,
        VERTICAL_ALIGN_MIDDLE,
        VERTICAL_ALIGN_BOTTOM,
        VERTICAL_ALIGN_NEXT_BOTTOM
    };

    // Todo: border_width enum 
    enum border_width {
        BORDER_WIDTH_THIN,
        BORDER_WIDTH_MEDIUM,
        BORDER_WIDTH_THICK
    };

    // Todo border style enum
    enum border_style {
        BORDER_STYLE_NONE,
        BORDER_STYLE_HIDDEN,
        BORDER_STYLE_DOTTED,
        BORDER_STYLE_DASHED,
        BORDER_STYLE_SOLID,
        BORDER_STYLE_DOUBLE,
        BORDER_STYLE_GROOVE,
        BORDER_STYLE_RIDGE,
        BORDER_STYLE_INSET,
        BORDER_STYLE_OUTSET
    };

    // Todo: element float enum
    enum element_float {
        FLOAT_NONE,
        FLOAT_LEFT,
        FLOAT_RIGHT
    };

    // Todo: element clear string

    enum element_clear {
        CLEAR_NONE,
        CLEAR_LEFT,
        CLEAR_RIGHT,
        CLEAR_BOTH
    };

    // Todo: css unit enum

    enum css_units {
        CSS_UNITS_NONE,
        CSS_UNITS_PERCENTAGE,
        CSS_UNITS_IN,
        CSS_UNITS_CM,
        CSS_UNITS_MM,
        CSS_UNITS_EM,
        CSS_UNITS_EX,
        CSS_UNITS_PT,
        CSS_UNITS_PC,
        CSS_UNITS_PX,
        CSS_UNITS_DPI,
        CSS_UNITS_DPCM,
        CSS_UNITS_VW,
        CSS_UNITS_VH,
        CSS_UNITS_VMIN,
        CSS_UNITS_VMAX
    };

    // Todo background attachment enum

    enum background_attachment {
        BACKGROUND_ATTACHMENT_SCROLL,
        BACKGROUND_ATTACHMENT_FIXED
    };


    // Todo: background repeat enum
    enum background_repeat {
        BACKGROUND_REPEAT_REPEAT,
        BACKGROUND_REPEAT_X,
        BACKGROUND_REPEAT_Y,
        BACKGROUND_REPEAT_NO_REPEAT
    };

    // Todo background box enum
    enum background_box {
        BACKGROUND_BOX_BORDER,
        BACKGROUND_BOX_PADDING,
        BACKGROUND_BOX_CONTENT
    };

    // Todo: element position enum
    enum element_position {
        ELEMENT_POSITION_STATIC,
        ELEMENT_POSITION_RELATIVE,
        ELEMENT_POSITION_ABSOLUTE,
        ELEMENT_POSITION_FIXED
    };

    // Todo: text align enum
    enum text_align {
        TEXT_ALIGN_LEFT,
        TEXT_ALIGN_RIGHT,
        TEXT_ALIGN_CENTER,
        TEXT_ALIGN_JUSTIFY
    };

    // Todo: text transform enum 
    enum text_transform {
        TEXT_TRANSFORM_NONE,
        TEXT_TRANSFORM_CAPITALIZE,
        TEXT_TRANSFORM_UPPERCASE,
        TEXT_TRANSFORM_LOWERCASE
    };

    // Todo: white space enum
    enum white_space {
        WHITE_SPACE_NORMAL,
        WHITE_SPACE_NOWRAP,
        WHITE_SPACE_PRE,
        WHITE_SPACE_PRE_LINE,
        WHITE_SPACE_PRE_WRAP
    };

    // Todo: overflow enum
    enum overflow {
        OVERFLOW_VISIBLE,
        OVERFLOW_HIDDEN,
        OVERFLOW_SCROLL,
        OVERFLOW_AUTO,
        OVERFLOW_NO_DISPLAY,
        OVERFLOW_NO_CONTENT
    };

    // Todo: background size enum
    enum background_size {
        BACKGROUND_SIZE_AUTO,
        BACKGROUND_SIZE_COVER,
        BACKGROUND_SIZE_CONTAIN
    };


    // Todo: visibility enum
    enum visibility {
        VISIBILITY_VISIBLE,
        VISIBILITY_HIDDEN,
        VISIBILITY_COLLAPSE
    };

    // Todo border collapse enum
    enum border_collapse {
        BORDER_COLLAPSE_COLLAPSE,
        BORDER_COLLAPSE_SEPARATE
    };

    // Todo: pseudo class enum 
    enum pseudo_class {
        PSEUDO_CLASS_ONLY_CHILD,
        PSEUDO_CLASS_ONLY_OF_TYPE,
        PSEUDO_CLASS_FIRST_CHILD,
        PSEUDO_CLASS_FIRST_OF_TYPE,
        PSEUDO_CLASS_LAST_CHILD,
        PSEUDO_CLASS_LAST_OF_TYPE,
        PSEUDO_CLASS_NTH_CHILD,
        PSEUDO_CLASS_NTH_OF_TYPE,
        PSEUDO_CLASS_NTH_LAST_CHILD,
        PSEUDO_CLASS_NTH_LAST_OF_TYPE,
        PSEUDO_CLASS_NOT,
        PSEUDO_CLASS_LANG
    };

    // Todo content property enum
    enum content_property {
        CONTENT_PROPERTY_NONE,
        CONTENT_PROPERTY_NORMAL,
        CONTENT_PROPERTY_OPEN_QUOTE,
        CONTENT_PROPERTY_CLOSE_QUOTE,
        CONTENT_PROPERTY_NO_OPEN_QUOTE,
        CONTENT_PROPERTY_NO_CLOSE_QUOTE
    };

    //Todo: media orientation enum
    enum media_orientation {
        MEDIA_ORIENTATION_PORTRAIT,
        MEDIA_ORIENTATION_LANDSCAPE
    };

    // Todo: media feature enum,
    enum media_feature {
        MEDIA_FEATURE_NONE,
        MEDIA_FEATURE_WIDTH,
        MEDIA_FEATURE_MIN_WIDTH,
        MEDIA_FEATURE_MAX_WIDTH,
        MEDIA_FEATURE_HEIGHT,
        MEDIA_FEATURE_MIN_HEIGHT,
        MEDIA_FEATURE_MAX_HEIGHT,
        MEDIA_FEATURE_DEVICE_WIDTH,
        MEDIA_FEATURE_MIN_DEVICE_WIDTH,
        MEDIA_FEATURE_MAX_DEVICE_WIDTH,
        MEDIA_FEATURE_DEVICE_HEIGHT,
        MEDIA_FEATURE_MIN_DEVICE_HEIGHT,
        MEDIA_FEATURE_MAX_DEVICE_HEIGHT,
        MEDIA_FEATURE_ORIENTATION,
        MEDIA_FEATURE_ASPECT_RATIO,
        MEDIA_FEATURE_MIN_ASPECT_RATIO,
        MEDIA_FEATURE_MAX_ASPECT_RATIO,
        MEDIA_FEATURE_DEVICE_ASPECT_RATIO,
        MEDIA_FEATURE_MIN_DEVICE_ASPECT_RATIO,
        MEDIA_FEATURE_MAX_DEVICE_ASPECT_RATIO,
        MEDIA_FEATURE_COLOR,
        MEDIA_FEATURE_MIN_COLOR,
        MEDIA_FEATURE_MAX_COLOR,
        MEDIA_FEATURE_MONOCHROME,
        MEDIA_FEATURE_MIN_MONOCHROME,
        MEDIA_FEATURE_MAX_MONOCHROME,
        MEDIA_FEATURE_RESOLUTION,
        MEDIA_FEATURE_MIN_RESOLUTION,
        MEDIA_FEATURE_MAX_RESOLUTION
    };

    //Todo: box sizing enum
    enum box_sizing {
        BOX_SIZING_CONTENT_BOX,
        MEDIA_FEATURE_BORDER_BOX
    };

    // Todo: media type enum
    enum media_type {
        MEDIA_TYPE_NONE,
        MEDIA_TYPE_ALL,
        MEDIA_TYPE_SCREEN,
        MEDIA_TYPE_PRINT,
        MEDIA_TYPE_BRAILLE,
        MEDIA_TYPE_EMBOSSED,
        MEDIA_TYPE_HANDHELD,
        MEDIA_TYPE_PROJECTION,
        MEDIA_TYPE_SPEECH,
        MEDIA_TYPE_TTY,
        MEDIA_TYPE_TV
    };

    enum select_result {
        SELECT_NO_MATCH             = 0x00,
        SELECT_MATCH                = 0x01,
        SELECT_MATCH_PSEUDO_CLASS   = 0x02,
        SELECT_MATCH_WITH_BEFORE    = 0x10,
        SELECT_MATCH_WITH_AFTER     = 0x20
    };

    enum render_type {
        RENDER_ALL,
        RENDER_NO_FIXED,
        RENDER_FIXED_ONLY
    }

    struct floated_box {
        typedef std::vector<floated_box> vector;
        position pos;
        element_float float_side;
        element_clear clear_float;
        std::shared_ptr<element> elements;


        floated_box() = default;
        floated_box(const floated_box &other) {
            pos = other.pos;
            float_side = other.float_side;
            clear_float = other.clear_float;
            elements = other.elements;
        }
        floated_box& operator=(const floated_box &other) {
            pos = other.pos;
            float_side = other.float_side;
            clear_float = other.clear_float;
            elements = other.elements;
            return *this;
        }
        floated_box(floated_box &&other) {
            pos = other.pos;
            float_side = other.float_side;
            clear_float = other.clear_float;
            elements = std::move(other.elements);
        }
        void operator=(floated_box &&val) {
            pos = other.pos;
            float_side = other.float_side;
            clear_float = other.clear_float;
            elements = std::move(other.elements);
        }
    };


    struct int_int_cache {
        int hash_value;
        int value;
        bool is_valid;
        bool is_default;

        int_int_cache() {
            hash_value = 0;
            value = 0;
            is_valid = false;
            is_default = false;
        }
        void invalidate() {
            is_valid = false;
            is_default = false;
        }
        void set_value(int hash_result , int value_result) {
            hash_value = hash_result;
            value = value_result;
            is_valid = true;
        }
    };


    template <class T>
    class def_value {
        T m_value;
        bool value_is_default;

        public:
            def_value(T default_value) {
                value_is_default = true;
                m_value = default_value;
            }
            void reset(T default_value) {
                value_is_default = true;
                m_value = default_value;
            }
            bool is_default() {
                return value_is_default;
            }
            T operator=(T new_value) {
                m_value = new_value;
                value_is_default = false;
                return m_value;
            }
            operator T() {
                return m_value;
            }
    };

    struct media_features {
        media_type type;
        // (pixels) For continuous media, 
        // this is the width of the viewport 
        // including the size of a rendered scroll bar (if any). 
        // For paged media, this is the width of the page box.
        int width;
        // (pixels) The height of the targeted display area 
        // of the output device. For continuous media, 
        // this is the height of the viewport 
        // including the size of a rendered scroll bar (if any). 
        // For paged media, this is the height of the page box.
        int height;
        // (pixels) The width of the rendering surface 
        // of the output device. For continuous media, 
        // this is the width of the screen. For paged media, 
        // this is the width of the page sheet size.
        int device_width;
        // (pixels) The height of the rendering surface of the output device. 
        // For continuous media, this is the height of the screen. 
        // For paged media, this is the height of the page sheet size.
        int device_height;
        // The number of bits per color component of the output device. 
        // If the device is not a color device, the value is zero.
        int color;
        // The number of entries in the color lookup table of the output device. 
        // If the device does not use a color lookup table, the value is zero.
        int color_index;
        // The number of bits per pixel in a monochrome frame buffer. 
        // If the device is not a monochrome device, the output device value will be 0.
        int monochrome;
        // The resolution of the output device (in DPI)
        int resolution;
    }

    #define style_display_strings           _t("none;block;inline;inline-block;inline-table;list-item;table;table-caption;table-cell;table-column;table-column-group;table-footer-group;table-header-group;table-row;table-row-group")
    #define font_size_strings               _t("xx-small;x-small;small;medium;large;x-large;xx-large;smaller;larger")
    #define font_style_strings              _t("normal;italic")
    #define font_variant_strings            _t("normal;small-caps")
    #define font_weight_strings             _t("normal;bold;bolder;lighter100;200;300;400;500;600;700")
    #define list_style_type_strings	        _t("none;circle;disc;square;armenian;cjk-ideographic;decimal;decimal-leading-zero;georgian;hebrew;hiragana;hiragana-iroha;katakana;katakana-iroha;lower-alpha;lower-greek;lower-latin;lower-roman;upper-alpha;upper-latin;upper-roman")
    #define list_style_position_strings	    _t("inside;outside")
    #define vertical_align_strings	        _t("baseline;sub;super;top;text-top;middle;bottom;text-bottom")
    #define border_width_strings	        _t("thin;medium;thick")
    #define border_style_strings	        _t("none;hidden;dotted;dashed;solid;double;groove;ridge;inset;outset")
    #define element_float_strings	        _t("none;left;right")
    #define element_clear_strings	        _t("none;left;right;both")
    #define css_units_strings	            _t("none;%;in;cm;mm;em;ex;pt;pc;px;dpi;dpcm;vw;vh;vmin;vmax")
    #define background_attachment_strings	_t("scroll;fixed")
    #define background_repeat_strings	    _t("repeat;repeat-x;repeat-y;no-repeat")
    #define background_box_strings	        _t("border-box;padding-box;content-box")
    #define element_position_strings	    _t("static;relative;absolute;fixed")
    #define text_align_strings		        _t("left;right;center;justify")
    #define text_transform_strings		    _t("none;capitalize;uppercase;lowercase")
    #define white_space_strings		        _t("normal;nowrap;pre;pre-line;pre-wrap")
    #define overflow_strings		        _t("visible;hidden;scroll;auto;no-display;no-content")
    #define background_size_strings		    _t("auto;cover;contain")
    #define visibility_strings			    _t("visible;hidden;collapse")
    #define border_collapse_strings		    _t("collapse;separate")
    #define pseudo_class_strings		    _t("only-child;only-of-type;first-child;first-of-type;last-child;last-of-type;nth-child;nth-of-type;nth-last-child;nth-last-of-type;not;lang")
    #define content_property_string		    _t("none;normal;open-quote;close-quote;no-open-quote;no-close-quote")
    #define media_orientation_strings		_t("portrait;landscape")
    #define media_feature_strings		    _t("none;width;min-width;max-width;height;min-height;max-height;device-width;min-device-width;max-device-width;device-height;min-device-height;max-device-height;orientation;aspect-ratio;min-aspect-ratio;max-aspect-ratio;device-aspect-ratio;min-device-aspect-ratio;max-device-aspect-ratio;color;min-color;max-color;color-index;min-color-index;max-color-index;monochrome;min-monochrome;max-monochrome;resolution;min-resolution;max-resolution")
    #define box_sizing_strings		        _t("content-box;border-box")
    #define media_type_strings		        _t("none;all;screen;print;braille;embossed;handheld;projection;speech;tty;tv")

    const simple_html::tchar_t* const void_elements = _t("area;base;br;col;command;embed;hr;img;input;keygen;link;meta;param;source;track;wbr");

};



#endif