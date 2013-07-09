// font.hpp: System that handles font requests. Dynamically searches for and loads fonts at request.

#ifndef IS_FONT_H_
#define IS_FONT_H_

#include <vector>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "lua.hpp"

namespace is {

class Font {
public:
    Font( std::string name, std::string dir, FT_Face font );
    std::string m_name;
    std::string m_dir;
    FT_Face     m_face;
    char*       m_data;
};

class FontLoader {
private:
    std::vector<is::Font>  m_fonts;
    void                        loadFont( int id );
public:
    FontLoader();
    ~FontLoader();
    int                         init();
    is::Font*              get( std::string fontname );
    void                        addFont( std::string name, std::string dir );
};

};

extern is::FontLoader* fonts;
extern FT_Library ftlib;

#endif // IS_FONT_H_
