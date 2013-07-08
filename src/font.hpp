// font.hpp: System that handles font requests. Dynamically searches for and loads fonts at request.

#ifndef IS_FONT_H_
#define IS_FONT_H_

#include <vector>
#include <string>
#include <SFML/Graphics/Font.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "lua.hpp"

namespace is {

class FontStore {
public:
    FontStore( std::string name, std::string dir, FT_Face font );
    std::string m_name;
    std::string m_dir;
    FT_Face     m_face;
    char*       m_data;
};

class Font {
private:
    std::vector<is::FontStore>  m_fonts;
    void                        loadFont( int id );
public:
    Font();
    ~Font();
    int                         init();
    is::FontStore*              get( std::string fontname );
    void                        addFont( std::string name, std::string dir );
};

};

extern is::Font* fonts;
extern FT_Library ftlib;

#endif // IS_FONT_H_
