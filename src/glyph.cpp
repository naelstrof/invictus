#include "glyph.hpp"

is::Glyph* glyph = new is::Glyph();

is::Glyph::Glyph() {
    m_texture = NULL;
}

is::Glyph::~Glyph() {
    if ( m_texture ) {
        delete m_texture;
    }
}

int is::Glyph::init() {
    return 0;
}

is::GlyphInfo* is::Glyph::get( sf::Uint32 id, std::string fontname, char style ) {
    // First search if we can find the specified font.
    for ( unsigned int i=0; i<m_glyphcontainers.size(); i++ ) {
        if ( fontname == m_glyphcontainers[i]->m_name ) {
            // Then find the glyph and return it
        }
    }
    // Now since the font doesn't exist, we must grab it and initialize it.
    is::GlyphContainer* gc = new is::GlyphContainer();
    is::FontStore* font = fonts->get( fontname );
    if ( !font ) {
        os->printf( "ERR The font % cannot be found!", fontname );
        return NULL;
    }
    gc->m_name = fontname ;

    return NULL;
}
