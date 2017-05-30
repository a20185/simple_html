#include "attributes.h"

namespace simple_html {
    class property_value {
        public:
            tstring m_value;
            bool important;
            property_value() {
                important = false;
            }
            property_value(const tchar_t *value , bool _important) {
                important = _important;
                m_value = value;
            }
            property_value(const property_value &other) {
                m_value = other.m_value;
                important = other.important;
            }
            property_value &operator=(const property_value &other) {
                m_value = other.m_value;
                important = other.important;
                return *this;
            }
    };

    typedef std::map<tstring , property_value> properties_map;

    class style {
        public:
            typedef std::shared_ptr<style> ptr;
            typedef std::vector<style::ptr> vector;
            style();
            style(const style &other);
            virtual ~style();
            void add_property(const tchar_t *name , const tchar_t *value , const tchar_t *baseurl , bool important);
            void combine(const simple_html::style &src);
            void operator=(const style &other) {
                m_properties = other.m_properties;
            }
            void add(const tchar_t *text , const tchar_t *baseurl) {
                parse(text , baseurl);
            }
            const tchar_t *get_property(const tchar_t *name) const {
                if (name) {
                    properties_map::const_iterator index = m_properties.find(name);
                    if (index != m_properties.end()) {
                        return index -> second.m_value;
                    }
                }
                return 0;
            }
            void clear() {
                m_properties.clear();
            }

        private:
            properties_map m_properties;
            static string_map m_validvalues;
            void parse_property(const tstring &text , const tchar_t *baseurl);
            void parse(const tchar_t *txt , const tchar_t *baseurl);
            void parse_short_border(const tstring &prefix , const tstring &value , bool important);
            void parse_short_background(const tstring &value , const tchar_t *baseurl , bool important);
            void parse_short_font(const tstring &value , bool important);
            void add_parsed_property(const tstring &name , const tstring &value , bool important);
            void remove_property(const tstring &name , bool important);
    }
};