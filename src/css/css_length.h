#ifndef SIMPLE_HTML_CSS_LENGTH_H__
#define SIMPLE_HTML_CSS_LENGTH_H__

#include "types.h"

namespace simple_html {
    class css_length {
        // Union represents the values
        union {
            float m_value;
            int m_predef;
        };
        css_units m_units;
        bool m_ispred;

        public:
            css_length();
            css_length(const css_length &other);
            css_length &operator=(const css_length &value);
            css_length &operator=(float value);
            bool is_predefined() const;
            void predef(int value);
            int predef() const;
            void set_value(float value , css_units units);
            float value() const;
            css_units units() const;
            int calc_percent(int width) const;
            void from_string(const tstring &string , const tstring &predefs = _t("") , int defined_value = 0);
    };

    inline css_length::css_length() {
        m_value = 0;
        m_predef = 0;
        m_units = css_units_none;
    }
    inline css_length::css_length(const css_length &value) {
        if (value.is_predefined()) {
            m_predef = value.m_predef;
        } else {
            m_value = value.m_value;
        }
        m_units = value.m_units;
        m_ispred = value.m_ispred;
    }
    inline css_length &css_lenfth::operator=(const css_length &value) {
        if (value.is_predefined()) {
            m_predef = value.m_predef;
        } else {
            m_value = value.m_value;
        }
        m_units = value.m_units;
        m_ispred = value.m_ispred;
        return *this;
    }

    inline css_length &css_length::operator=(float value) {
        m_value = value;
        m_units = css_units_px;
        m_ispred = false;
        return *this;
    }
} 



#endif