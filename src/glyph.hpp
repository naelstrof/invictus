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
class Glyph {
public:
    Glyph( is::Font* font, int size, sf::String id, is::TextureAtlas* texture );
    sf::String              m_id;
    int                     m_size;
    bool                    m_renderable;
    float                   m_advanceX;
    float                   m_advanceY;
    float                   m_bitmapWidth;
    float                   m_bitmapHeight;
    float                   m_bitmapLeft;
    float                   m_bitmapTop;
    void                    fixUV( is::TextureAtlas* texture );
    glm::vec2               m_uv[4];
private:
    is::TextureAtlas::Node* m_node;
    float                   m_texturesize;
};

// Stores a set of glyph information.
class GlyphsContainer {
private:
    is::Glyph*              add( sf::String id, int size );
public:
    GlyphsContainer( is::Font* font, int textureSize );
    ~GlyphsContainer();
    std::vector<is::Glyph>  m_glyphs;
    // Must be a pointer because otherwise it gets deconstructed in some place i couldn't find.
    is::TextureAtlas*        m_texture;
    is::Font*               m_font;
    is::Glyph*              find( sf::String id, int size );
};

class GlyphLoader {
private:
    unsigned int        m_textureStartSize;
    std::vector<GlyphsContainer*> m_glyphcontainers;
public:
    ~GlyphLoader();
    is::Glyph*          get( sf::String id, std::string font, int size );
    is::TextureAtlas*   getTexture( std::string font );
};

};

extern is::GlyphLoader* glyphs;

#endif // IS_GLYPH_H_
