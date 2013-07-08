// glyph.hpp: Handles rendering and generating glyph information.

#ifndef IS_GLYPH_H_
#define IS_GLYPH_H_

#include <vector>
#include <string>

#include "texture_atlas.hpp"

namespace is {

// Stores a single glyph's information
class GlyphInfo {
    // Not sure what we have to store yet :u
};

// Stores a set of glyph information.
class GlyphsContainer {
    std::vector<GlyphInfo>      m_glyphs;
    std::vector<sf::Uint32>     m_id;
    is::TextureAtlas*           m_texture;
    std::string                 m_name;
};

class Glyph {
private:
    std::vector<GlyphsContainer*> m_glyphcontainers;
public:
    Glyph();
    ~Glyph();
    int             init();
    is::GlyphInfo*  get( sf::Uint32 id, std::string font, char style );
};

};

extern is::Glyph* glyph;

#endif // IS_GLYPH_H_
