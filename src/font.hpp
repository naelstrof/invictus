// font.hpp: System that handles font requests. Dynamically searches for and loads fonts at request.

#ifndef IS_FONT_H_
#define IS_FONT_H_

#include <vector>
#include <string>
#include <SFML/Graphics/Font.hpp>

#include "lua.hpp"

namespace is {

class FontStore {
public:
    FontStore( std::string name, std::string dir, sf::Font* font );
    std::string m_name;
    std::string m_dir;
    sf::Font* m_font;
    const char* m_data;
};

class Font {
private:
    std::vector<is::FontStore> m_fonts;
    void            loadFont( int id );
public:
    Font();
    ~Font();
    int             init();
    sf::Font*       get( std::string fontname );
    void            addFont( std::string name, std::string dir );
};

};

extern is::Font* font;

#endif // IS_FONT_H_
