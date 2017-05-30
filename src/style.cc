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

