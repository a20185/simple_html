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
        } else if (!t_strcmp(name , -t("border-bottom-right-radius"))) {
            string_vector tokens;
            split_string(value , tokens , _t(" "));
            if (tokens.size() >= 2) {
                add_property(_t("border-bottom-right-radius-x") , tokens[0].c_str() , baseurl , importance);
                add_property(_t("border-bottom-right-radius-y") , tokens[1].c_str() , baseurl , importance);
            } else if (tokens.size() == 1) {
                add_property(_t("border-bottom-right-radius-x") , tokens[0].c_str() , baseurl , importance);
                add_property(_t("border-bottom-right-radius-y") , tokens[0].c_str() , baseurl , importance);
            }
        } else if (!t_strcmp(name , _t("border-top-left-radius"))) {
            string_vector tokens;
            split_string(value , tokens , _t(" "));
            if (tokens.size() >= 2) {
                add_property(_t("border-top-left-radius-x") , tokens[0].c_str() , baseurl , importance);
                add_property(_t("border-top-left-radius-y") , tokens[1].c_str() , baseurl , importance);
            } else if (tokens.size() == 1) {
                add_property(_t("border-top-left-radius-x") , tokens[0].c_str() , baseurl , importance);
                add_property(_t("border-top-left-radius-y") , tokens[0].c_str() , baseurl , importance);
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
                add_property(_t("border-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-radius-y"), tokens[0].c_str(), baseurl, importance);
            } else if(tokens.size() >= 2) {
                add_property(_t("border-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-radius-y"), tokens[1].c_str(), baseurl, importance);
            }
        } else if (!t_strcmp(name , _t("border-radius-x"))) {
            string_vector tokens;
            split_string(value, tokens, _t(" "));
            if(tokens.size() == 1) {
                add_property(_t("border-top-left-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-top-right-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-bottom-right-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-bottom-left-radius-x"), tokens[0].c_str(), baseurl, importance);
            } else if(tokens.size() == 2) {
                add_property(_t("border-top-left-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-top-right-radius-x"), tokens[1].c_str(), baseurl, importance);
                add_property(_t("border-bottom-right-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-bottom-left-radius-x"), tokens[1].c_str(), baseurl, importance);
            } else if(tokens.size() == 3) {
                add_property(_t("border-top-left-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-top-right-radius-x"), tokens[1].c_str(), baseurl, importance);
                add_property(_t("border-bottom-right-radius-x"), tokens[2].c_str(), baseurl, importance);
                add_property(_t("border-bottom-left-radius-x"), tokens[1].c_str(), baseurl, importance);
            } else if(tokens.size() == 4) {
                add_property(_t("border-top-left-radius-x"), tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-top-right-radius-x"), tokens[1].c_str(), baseurl, importance);
                add_property(_t("border-bottom-right-radius-x"), tokens[2].c_str(), baseurl, importance);
                add_property(_t("border-bottom-left-radius-x"), tokens[3].c_str(), baseurl, importance);
            }
        } else if (!t_strcmp(name , _t("border-radius-y"))) {
            string_vector tokens;
            split_string(value, tokens, _t(" "));
            if(tokens.size() == 1) {
                add_property(_t("border-top-left-radius-y"),		tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-top-right-radius-y"),		tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-bottom-right-radius-y"),	tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-bottom-left-radius-y"),	tokens[0].c_str(), baseurl, importance);
            } else if(tokens.size() == 2) {
                add_property(_t("border-top-left-radius-y"),		tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-top-right-radius-y"),		tokens[1].c_str(), baseurl, importance);
                add_property(_t("border-bottom-right-radius-y"),	tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-bottom-left-radius-y"),	tokens[1].c_str(), baseurl, importance);
            } else if(tokens.size() == 3) {
                add_property(_t("border-top-left-radius-y"),		tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-top-right-radius-y"),		tokens[1].c_str(), baseurl, importance);
                add_property(_t("border-bottom-right-radius-y"),	tokens[2].c_str(), baseurl, importance);
                add_property(_t("border-bottom-left-radius-y"),	tokens[1].c_str(), baseurl, importance);
            } else if(tokens.size() == 4) {
                add_property(_t("border-top-left-radius-y"),		tokens[0].c_str(), baseurl, importance);
                add_property(_t("border-top-right-radius-y"),		tokens[1].c_str(), baseurl, importance);
                add_property(_t("border-bottom-right-radius-y"),	tokens[2].c_str(), baseurl, importance);
                add_property(_t("border-bottom-left-radius-y"),	tokens[3].c_str(), baseurl, importance);
            }
        }
        if (!t_strcmp(name , _t("list-style"))) {
             add_parsed_property(_t("list-style-type") , _t("disc") , importance);
             add_parsed_property(_t("list-style-position") , _t("outside") , importance);
             add_parsed_property(_t("list-style-image") , _t("") , importance);
             add_parsed_property(_t("list-style-image-baseurl") , _t("") , importance);

             string_vector tokens;
             split_string(value , tokens , _t(" ") , _t("") , _t("("));
             for (string_vector::iterator it = tokens.begin() ; it != tokens.end() ; it++) {
                 int index = value_index(it -> c_str() , list_style_type_strings , -1);
                 if (index >= 0) {
                     add_parsed_property(_t("list-style-type") , *it , importance);
                 } else {
                     index = value_index(it -> c_str() , list_style_position_strings , -1);
                     if (index >= 0) {
                         add_parsed_property(_t("list-style-position") , *it , importance);
                     } else if (!t_strncmp(value , _t("url") , 3)) {
                         add_parsed_property(_t("list-style-image") , *it , importance);
                         if (baseurl) {
                             add_parsed_property(_t("list-style-image-baseurl") , baseurl , importance);
                         }
                     }
                 }
             }
        } else if (!t_strcmp(name , _t("list-style-image"))) {
            add_parsed_property(name , value , importance);
            if (baseurl) {
                add_parsed_property(_t("list-style-image-baseurl") , baseurl , importance);
            }
        } else if (!t_strcmp(name , _t("background"))) {
            parse_short_background(value , baseurl , importance);
        } else if (!t_strcmp(name , _t("margin")) || !t_strcmp(name , _t("padding"))) {
            string_vector tokens;
            split_string(value , tokens , _t(" "));
            if (tokens.size() >= 4) {
                add_parsed_property(tstring(name) + _t("-top"), tokens[0], importance);
                add_parsed_property(tstring(name) + _t("-right"), tokens[1], importance);
                add_parsed_property(tstring(name) + _t("-bottom"), tokens[2], importance);
                add_parsed_property(tstring(name) + _t("-left"), tokens[3], importance);
            } else if(tokens.size() == 3) {
                add_parsed_property(tstring(name) + _t("-top"), tokens[0], importance);
                add_parsed_property(tstring(name) + _t("-right"), tokens[1], importance);
                add_parsed_property(tstring(name) + _t("-left"), tokens[1], importance);
                add_parsed_property(tstring(name) + _t("-bottom"), tokens[2], importance);
            } else if(tokens.size() == 2) {
                add_parsed_property(tstring(name) + _t("-top"), tokens[0], importance);
                add_parsed_property(tstring(name) + _t("-bottom"), tokens[0], importance);
                add_parsed_property(tstring(name) + _t("-right"), tokens[1], importance);
                add_parsed_property(tstring(name) + _t("-left"), tokens[1], importance);
            } else if(tokens.size() == 1) {
                add_parsed_property(tstring(name) + _t("-top"), tokens[0], importance);
                add_parsed_property(tstring(name) + _t("-bottom"), tokens[0], importance);
                add_parsed_property(tstring(name) + _t("-right"), tokens[0], importance);
                add_parsed_property(tstring(name) + _t("-left"), tokens[0], importance);
            }
        } else if (!t_strcmp(name, _t("border-left")) || !t_strcmp(name, _t("border-right")) ||
                    !t_strcmp(name, _t("border-top"))  || !t_strcmp(name, _t("border-bottom"))) {
            parse_short_border(name , value , importance);
        } else if (!t_strcmp(name, _t("border-width")) || !t_strcmp(name, _t("border-style")) || !t_strcmp(name, _t("border-color"))) {
            string_vector nametokens;
            split_string(name, nametokens, _t("-"));
            string_vector tokens;
            split_string(value, tokens, _t(" "));
            if (tokens.size() >= 4) {
                add_parsed_property(nametokens[0] + _t("-top-")		+ nametokens[1], tokens[0], importance);
                add_parsed_property(nametokens[0] + _t("-right-")	+ nametokens[1], tokens[1], importance);
                add_parsed_property(nametokens[0] + _t("-bottom-")	+ nametokens[1], tokens[2], importance);
                add_parsed_property(nametokens[0] + _t("-left-")	+ nametokens[1], tokens[3], importance);
            } else if (tokens.size() == 3) {
                add_parsed_property(nametokens[0] + _t("-top-")		+ nametokens[1], tokens[0], importance);
                add_parsed_property(nametokens[0] + _t("-right-")	+ nametokens[1], tokens[1], importance);
                add_parsed_property(nametokens[0] + _t("-left-")	+ nametokens[1], tokens[1], importance);
                add_parsed_property(nametokens[0] + _t("-bottom-")	+ nametokens[1], tokens[2], importance);
            } else if(tokens.size() == 2) {
                add_parsed_property(nametokens[0] + _t("-top-")		+ nametokens[1], tokens[0], importance);
                add_parsed_property(nametokens[0] + _t("-bottom-")	+ nametokens[1], tokens[0], importance);
                add_parsed_property(nametokens[0] + _t("-right-")	+ nametokens[1], tokens[1], importance);
                add_parsed_property(nametokens[0] + _t("-left-")	+ nametokens[1], tokens[1], importance);
            } else if(tokens.size() == 1) {
                add_parsed_property(nametokens[0] + _t("-top-")		+ nametokens[1], tokens[0], importance);
                add_parsed_property(nametokens[0] + _t("-bottom-")	+ nametokens[1], tokens[0], importance);
                add_parsed_property(nametokens[0] + _t("-right-")	+ nametokens[1], tokens[0], importance);
                add_parsed_property(nametokens[0] + _t("-left-")	+ nametokens[1], tokens[0], importance);
            }
        } else if(!t_strcmp(name, _t("font"))) {
            parse_short_font(value, importance);
        } else {
            add_parsed_property(name, value, importance);
        }
    }
}

void simple_html::style::parse_short_border(const tstring &prefix , const tstring &value , bool important) {
    string_vector tokens;
    split_string(value , tokens , _t(" ") , _t("") , _t("("));
    if (tokens.size() >= 3) {
        add_parsed_property(prefix + _t("-width") , tokens[0] , important);
        add_parsed_property(prefix + _t("-style") , tokens[1] , important);
        add_parsed_property(prefix + _t("-color") , tokens[2] , important);
    } else if (tokens.size() == 2) {
        if (iswdigit(tokens[0][0]) || value_index(value.c_str() , border_width_strings) >= 0) {
            add_parsed_property(prefix + _t("-width") , tokens[0] , important);
            add_parsed_property(prefix + _t("-style") , tokens[1] , important);
        } else {
            add_parsed_property(prefix + _t("-style") , tokens[0] , important);
            add_parsed_property(prefix + _t("-color") , tokens[1] , important);
        }
    }
}

void simple_html::style::parse_short_background(const tstring &value , const tchar_t *baseurl , bool important) {
    // Initialize the property with default values
    // In case error happens
    add_parsed_property(_t("background-color"), _t("transparent"), important);
	add_parsed_property(_t("background-image"), _t(""), important);
	add_parsed_property(_t("background-image-baseurl"), _t(""), important);
	add_parsed_property(_t("background-repeat"), _t("repeat"), important);
	add_parsed_property(_t("background-origin"), _t("padding-box"),	important);
	add_parsed_property(_t("background-clip"), _t("border-box"), important);
	add_parsed_property(_t("background-attachment"), _t("scroll"), important);

    if (value == _t("none")) {
        return;
    }

    string_vector tokens;
    split_string(value , tokens , _t(" ") , _t("") , _t("("));
    bool origin_found = false;

    for (string_vector::iterator it = tokens.begin(); it != tokens.end() ; it++) {
        if (it -> substr(0 , 3) == _t("url")) {
            add_parsed_property(_t("background-image") , *it , important);
            if (baseurl) {
                add_parsed_property(_t("background-image-baseurl") , baseurl , important);
            }
        } else if (value_in_list(it -> c_str() , background_repeat_strings)) {
            add_parsed_property(_t("background-repeat") , *it , important);
        } else if (value_in_list(it -> c_str() , background_attachment_strings)) {
            add_parsed_property(_t("background-attachment") , *it , important);
        } else if (value_in_list(it -> c_str() , background_box_strings)) {
            if (!origin_found) {
                add_parsed_property(_t("background-origin") , *it , important);
                origin_found = true;
            } else {
                add_parsed_property(_t("background-clip") , *it , important);
            }
        } else if (value_in_list(it -> c_str() , _t("left;right;top;bottom;center")) ||
                   iswdigit((*it)[0]) || (*it)[0] == _t('-') || (*it)[0] == _t('.') || (*it)[0] == _t('.')) {
            if (m_properties.find(_t("background-position")) != m_properties.end()) {
                m_properties[_t("background-position")].m_value = m_properties[_t("background-position")].m_value + _t(" ") + *it;
            } else {
                add_parsed_property(_t("background-position") , *it , important);
            }
        } else if (web_color::is_color(it -> c_str())) {
            add_parsed_property(_t("background-color") , *it , important);
        }
    }
}

void simple_html::style::parse_short_font(const tstring &value , bool important) {
    // Set to default
    add_parsed_property(_t("font-style"), _t("normal"), important);
    add_parsed_property(_t("font-variant"), _t("normal"), important);
    add_parsed_property(_t("font-weight"), _t("normal"), important);
    add_parsed_property(_t("font-size"), _t("medium"), important);
    add_parsed_property(_t("line-height"), _t("normal"), important);

    string_vector tokens;
    split_string(value , tokens , _t(" ") , _t("") , _t("\""));
    int index = 0;
    bool normal = false;
    bool family = false;
    tstring font_family;

    for (string_vector::iterator it = tokens.begin() ; it != tokens.end() ; it++) {
        index = value_index(it -> c_str() , font_style_strings);
        if (!family) {
            if (index >= 0) {
                if (index == 0 && !normal) {
                    add_parsed_property(_t("font-weight") , *it , important);
                    add_parsed_property(_t("font-variant") , *it , important);
                    add_parsed_property(_t("font-style") , *it , important);
                } else {
                    add_parsed_property(_t("font-style") , *it , important);
                }
            } else {
                if (value_in_list(it -> c_str() , font_weight_strings)) {
                    add_parsed_property(_t("font-weight") , *it , important);
                } else {
                    if (value_in_list(it -> c_str() , font_variant_strings)) {
                        add_parsed_property(_t("font-variant") , *it , important);
                    } else if (iswdigit((*it)[0])) {
                        string_vector size_and_lineheight;
                        split_string(*it , size_and_lineheight , _t("/"));
                        if (size_and_lineheight.size() == 1) {
                            add_parsed_property(_t("font-size") , size_and_lineheight[0] , important);
                        } else if (size_and_lineheight.size() >= 2) {
                            add_parsed_property(_t("line-height") , size_and_lineheight[1] , important);
                        }
                    } else {
                        family = true;
                        font_family += *it;
                    }
                }
            }
        } else {
            font_family += *it;
        }
    }
    add_parsed_property(_t("font-family") , font_family , important);
}



void simple_html::add_parsed_property(const tstring &name , const tstring &value , bool important) {
    bool isvalid = true;
    string_map::iterator values = m_validvalues.find(name);
    if (values != m_validvalues.end()) {
        if (!value_in_list(value , values -> second)) {
            isvalid = false;
        }
    }
    if (isvalid) {
        properties_map::iterator prop = m_properties.find(name);
        if (prop != m_properties.end()) {
            if (!prop -> second.important || (important && prop -> second.important)) {
                m_properties.erase(prop);
            }
        }
    }
}