#include <algorithm>
#include <functional>

#ifndef WINCE
#include <locale>
#endif

#include "style.h"
#include "html.h"

simple_html::string_map simple_html::style::m_validvalues = {
    {_t("white-space") , white_space_strings}
};

simple_html::style::style() {
    // Initialize later
}

simple_html::style::style(const style &other) {
    m_properties = other.m_properties;
}

simple_html::style::~style() {
    // Destruct later
}


// Main parse function
void simple_html::style::parse(const tchar_t *text , const tchar_t *baseurl) {
    std::vector<tstring> properties;
    split_string(text , properties , _t(";"));
    for (std::vector<tstring>::const_iterator i = properties.begin() ; i != properties.end() ; i++) {
        parse_property(*i , baseurl);
    }
}

void simple_html::style::parse_property(const tstring &text , const tchar_t *baseurl) {
    tstring::size_type pos = text.find_first_of(_t(":"));
    if (pos != tstring::npos) {
        // If found
        tstring name = text.substr(0 , pos);
        tstring val = text.substr(pos + 1);
        trim(name);
        trim(val);
        lcase(name);

        // if not empty
        if (!name.empty() && !val.empty()) {
            string_vector values;
            split_string(val , values , _t("!"));
            // If only one attribute property
            if (values.size() == 1) {
                add_property(name.c_str() , val.c_str() , baseurl , false);
            } else if (values.size() > 1) {
                trim(values[0]);
                lcase(values[1]);
                if (values[1] == _t("important")) {
                    add_property(name.c_str() , vals[0].c_str() , baseurl , true);
                } else {
                    add_property(name.c_str() , vals[0].c_str() , baseurl , false);
                }
            }
        }
    }
}


void simple_html::style::combine(const simple_html::style &source) {
    for (properties_map::const_iterator it = source.m_properties.begin() ; i != source.m_properties.end() ; i++) {
        add_parsed_property(i -> first.c_str() , i -> second.c_str() , i -> second.important);
    }
}


void simple_html::style::add_property(const tchar_t *name , const tchar_t *value , const tchar_t *baseurl , bool importance) {
    if (!name || !val) return;

    // Add baseurl for background_image
    if (!t_strcmp(name , _t("background-image"))) {
        add_parsed_property(name , val , importance);
        if (baseurl) {
            add_parsed_property(_t("background-image-baseurl") , baseurl , importance);
        }
    } else {
        // Parse border spacing properties
        if (!t_strcmp(name , _t("border-spacing"))) {
            string_vector tokens;
            split_string(value , tokens , _t(" "));
            if (tokens.size() == 1) {
                add_property(_t("-simple-html-border-spacing-x") , tokens[0].c_str() , baseurl , importance);
                add_property(_t("-simple-html-border-spacing-y") , tokens[0].c_str() , baseurl , importance);
            } else if (tokens.size() == 2) {
                add_property(_t("-simple-html-border-spacing-x") , tokens[0].c_str() , baseurl , importance);
                add_property(_t("-simple-html-border-spacing-y") , tokens[1].c_str() , baseurl , importance);
            }
        // Parse border-related properties
        // Include style width and color
        } else if (!strcmp(name , _t("border"))) {
            string_vector tokens;
            split_string(value , tokens , _t(" ") , _t("") , _t("("));
            int index;
            tstring temp_string;
            for (string_vector::const_iterator it = tokens.begin() ; it != tokens.end() ; it++) {
                index = value_index(it -> c_str() , border_style_strings , -1);
                if (index >= 0) {
                    add_property(_t("border-left-style") , it -> c_str() , baseurl , importance);
                    add_property(_t("border-right-style") , it -> c_str() , baseurl , importance);
                    add_property(_t("border-top-style") , it -> c_str() , baseurl , importance);
                    add_property(_t("border-bottom-style") , it -> c_str() , baseurl , importance);
                } else {
                    if (t_isdigit((*it)[0]) || (*it)[0] == _t('.') || value_in_list((*it) , _t("thin;medium;thick"))) {
                        add_property(_t("border-left-width") , it -> c_str() , baseurl , importance);
                        add_property(_t("border-right-width") , it -> c_str() , baseurl , importance);
                        add_property(_t("border-top-width") , it -> c_str() , baseurl , importance);
                        add_property(_t("border-bottom-width") , it -> c_str() , baseurl , importance);
                    } else {
                        add_property(_t("border-left-color") , it -> c_str() , baseurl , importance);
                        add_property(_t("border-right-color") , it -> c_str() , baseurl , importance);
                        add_property(_t("border-top-color") , it -> c_str() , baseurl , importance);
                        add_property(_t("border-bottom-color") , it -> c_str() , baseurl , importance);
                    }
                }
            }
        // If not contains all border properties ,then we separate parsing it.
        // Eg. if has 3 or 2 order attributes
        // 
        } else if (!t_strcmp(name , _t("border-left")) || !t_strcmp(name , _t("border-right")) ||
                   !t_strcmp(name , _t("border-top")) || !t_strcmp(name , _t("border-bottom"))) {
            string_vector tokens;
            split_string(value , tokens , _t(" ") , _t("") , _t("("));
            int index;
            tstring temp_string;
            for (string_vector::const_iterator it = tokens.begin() ; it != tokens.end() ; it++) {
                index = value_index(it -> c_str() , border_style_strings , -1);
                if (index >= 0) {
                    temp_string = name;
                    temp_string += _t("-style");
                    add_property(temp_string.c_str() , it -> c_str() , baseurl , importance);
                } else {
                    if (web_color::is_color(it -> c_str())) {
                        temp_string = name;
                        temp_string += _t("-color");
                        add_property(temp_string.c_str() , it -> c_str() , baseurl , importance);
                    } else {
                        temp_string = name;
                        temp_string += _t("-width");
                        add_property(temp_string.c_str() , it -> c_str() , baseurl , importance);
                    }
                }
            }
        // Parse the border radius
         } else if (!t_strcmp(name , -t("border-bottom-left-radius"))) {
             string_vector tokens;
             split_string(value , tokens , _t(" "));
             if (tokens.size() >= 2) {
                 add_property(_t("border-bottom-left-radius-x") , tokens[0].c_str() , baseurl , importance);
                 add_property(_t("border-bottom-left-radius-y") , tokens[1].c_str() , baseurl , importance);
             } else if (tokens.size() == 1) {
                 add_property(_t("border-bottom-left-radius-x") , tokens[0].c_str() , baseurl , importance);
                 add_property(_t("border-bottom-left-radius-y") , tokens[0].c_str() , baseurl , importance);
             }
         } else if (!t_strcmp(name , _t("border-top-right-radius"))) {
             string_vector tokens;
             split_string(value , tokens , _t(" "));
             if (tokens.size() >= 2) {
                 add_property(_t("border-top-right-radius-x") , tokens[0].c_str() , baseurl , importance);
                 add_property(_t("border-top-right-radius-y") , tokens[1].c_str() , baseurl , importance);
             } else if (tokens.size() == 1) {
                 add_property(_t("border-top-right-radius-x") , tokens[0].c_str() , baseurl , importance);
                 add_property(_t("border-top-right-radius-y") , tokens[0].c_str() , baseurl , importance);
             }
        // Parse the border radius shorthand-properties
         } else if (!t_strcmp(name , _t("border-radius"))) {
            string_vector tokens;
            split_string(value, tokens, _t("/"));
            if (tokens.size() == 1) {
                add_property(_t("border-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-radius-y"), tokens[0].c_str(), baseurl, important);
            } else if(tokens.size() >= 2) {
                add_property(_t("border-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-radius-y"), tokens[1].c_str(), baseurl, important);
            }
         } else if (!t_strcmp(name , _t("border-radius-x"))) {
            string_vector tokens;
            split_string(value, tokens, _t(" "));
            if(tokens.size() == 1) {
                add_property(_t("border-top-left-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-top-right-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-bottom-right-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-bottom-left-radius-x"), tokens[0].c_str(), baseurl, important);
            } else if(tokens.size() == 2) {
                add_property(_t("border-top-left-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-top-right-radius-x"), tokens[1].c_str(), baseurl, important);
                add_property(_t("border-bottom-right-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-bottom-left-radius-x"), tokens[1].c_str(), baseurl, important);
            } else if(tokens.size() == 3) {
                add_property(_t("border-top-left-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-top-right-radius-x"), tokens[1].c_str(), baseurl, important);
                add_property(_t("border-bottom-right-radius-x"), tokens[2].c_str(), baseurl, important);
                add_property(_t("border-bottom-left-radius-x"), tokens[1].c_str(), baseurl, important);
            } else if(tokens.size() == 4) {
                add_property(_t("border-top-left-radius-x"), tokens[0].c_str(), baseurl, important);
                add_property(_t("border-top-right-radius-x"), tokens[1].c_str(), baseurl, important);
                add_property(_t("border-bottom-right-radius-x"), tokens[2].c_str(), baseurl, important);
                add_property(_t("border-bottom-left-radius-x"), tokens[3].c_str(), baseurl, important);
            }
         } else if (!t_strcmp(name , _t("border-radius-y"))) {
            string_vector tokens;
            split_string(value, tokens, _t(" "));
            if(tokens.size() == 1) {
                add_property(_t("border-top-left-radius-y"),		tokens[0].c_str(), baseurl, important);
                add_property(_t("border-top-right-radius-y"),		tokens[0].c_str(), baseurl, important);
                add_property(_t("border-bottom-right-radius-y"),	tokens[0].c_str(), baseurl, important);
                add_property(_t("border-bottom-left-radius-y"),	tokens[0].c_str(), baseurl, important);
            } else if(tokens.size() == 2) {
                add_property(_t("border-top-left-radius-y"),		tokens[0].c_str(), baseurl, important);
                add_property(_t("border-top-right-radius-y"),		tokens[1].c_str(), baseurl, important);
                add_property(_t("border-bottom-right-radius-y"),	tokens[0].c_str(), baseurl, important);
                add_property(_t("border-bottom-left-radius-y"),	tokens[1].c_str(), baseurl, important);
            } else if(tokens.size() == 3) {
                add_property(_t("border-top-left-radius-y"),		tokens[0].c_str(), baseurl, important);
                add_property(_t("border-top-right-radius-y"),		tokens[1].c_str(), baseurl, important);
                add_property(_t("border-bottom-right-radius-y"),	tokens[2].c_str(), baseurl, important);
                add_property(_t("border-bottom-left-radius-y"),	tokens[1].c_str(), baseurl, important);
            } else if(tokens.size() == 4) {
                add_property(_t("border-top-left-radius-y"),		tokens[0].c_str(), baseurl, important);
                add_property(_t("border-top-right-radius-y"),		tokens[1].c_str(), baseurl, important);
                add_property(_t("border-bottom-right-radius-y"),	tokens[2].c_str(), baseurl, important);
                add_property(_t("border-bottom-left-radius-y"),	tokens[3].c_str(), baseurl, important);
            }
         }
    }
}

