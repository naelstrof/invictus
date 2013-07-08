// glyph.hpp: Handles rendering and generating glyph information.

#ifndef IS_GLYPH_H_
#define IS_GLYPH_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/System/String.hpp>

#include "texture_atlas.hpp"
#include "font.hpp"

namespace is {

// Stores a single glyph's information
class GlyphInfo {
public:
    GlyphInfo( is::FontStore* font, int size, sf::String id, is::TextureAtlas* texture );
    sf::String      m_id;
    int             m_size;
    bool            m_renderable;
    float           m_advanceX;
    float           m_advanceY;
    float           m_bitmapWidth;
    float           m_bitmapHeight;
    float           m_bitmapLeft;
    float           m_bitmapTop;
    glm::vec2       m_uv[4];
};

// Stores a set of glyph information.
class GlyphsContainer {
private:
    is::GlyphInfo*              add( sf::String id, int size );
public:
    GlyphsContainer( is::FontStore* font, int textureSize );
    std::vector<is::GlyphInfo>  m_glyphs;
    is::TextureAtlas            m_texture;
    is::FontStore*              m_font;
    is::GlyphInfo*              find( sf::String id, int size );
};

class Glyph {
private:
    std::vector<GlyphsContainer*> m_glyphcontainers;
public:
    ~Glyph();
    is::GlyphInfo*      get( sf::String id, std::string font, int size );
    is::TextureAtlas*   getTexture( std::string font );
};

};

extern is::Glyph* glyphs;

#endif // IS_GLYPH_H_
