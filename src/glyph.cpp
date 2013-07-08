#include "glyph.hpp"

is::Glyph* glyphs = new is::Glyph();

is::Glyph::~Glyph() {
    for ( unsigned int i=0; i<m_glyphcontainers.size(); i++ ) {
        delete m_glyphcontainers[i];
    }
}

is::TextureAtlas* is::Glyph::getTexture( std::string fontname ) {
    for ( unsigned int i=0; i<m_glyphcontainers.size(); i++ ) {
        if ( fontname == m_glyphcontainers[i]->m_font->m_name ) {
            return &( m_glyphcontainers[i]->m_texture );
        }
    }
    return NULL;
}

is::GlyphInfo* is::Glyph::get( sf::String id, std::string fontname, int size ) {
    // Make sure the string is only one character long
    if ( id.getSize() > 1 ) {
        os->printf( "ERR Unexpected call to is::Glyph::get() with id containing more than one character!\n" );
        return NULL;
    }
    // Then search if we can find the specified font.
    for ( unsigned int i=0; i<m_glyphcontainers.size(); i++ ) {
        if ( fontname == m_glyphcontainers[i]->m_font->m_name ) {
            is::GlyphInfo* info = m_glyphcontainers[i]->find( id, size );
            if ( !info ) {
                std::string text;
                sf::Utf<32>::toUtf8( id.begin(), id.end(), back_inserter( text ) );
                os->printf( "ERR The glyph % cannot be found!", text );
                return NULL;
            }
            return info;
        }
    }
    // Now since the font doesn't exist, we must grab it and initialize it.
    is::FontStore* font = fonts->get( fontname );
    if ( !font ) {
        os->printf( "ERR The font % cannot be found!", fontname );
        return NULL;
    }
    m_glyphcontainers.push_back( new is::GlyphsContainer( font, 1024 ) );
    return m_glyphcontainers.back()->find( id, size );
}

is::GlyphsContainer::GlyphsContainer( is::FontStore* font, int textureSize ) {
    m_font = font;
    m_texture = is::TextureAtlas( textureSize, textureSize );
}

is::GlyphInfo* is::GlyphsContainer::add( sf::String id, int size ) {
    m_glyphs.push_back( is::GlyphInfo( m_font, size, id, &m_texture ) );
    return &( m_glyphs.back() );
}

is::GlyphInfo* is::GlyphsContainer::find( sf::String id, int size ) {
    for ( unsigned int i=0; i<m_glyphs.size(); i++ ) {
        if ( m_glyphs[i].m_id == id && m_glyphs[i].m_size == size ) {
            return &( m_glyphs[i] );
        }
    }
    return add( id, size );
}

is::GlyphInfo::GlyphInfo( is::FontStore* font, int size, sf::String id, is::TextureAtlas* texture ) {
    m_renderable = false;
    m_size = size;
    m_id = id;
    // Set the height of the glyph in pixels.
    FT_Set_Pixel_Sizes( font->m_face, 0, m_size );
    // FIXME: This conversion could lose data on certain compilers I think.
    const sf::Uint32* dat = id.getData();
    FT_Load_Char( font->m_face, ( unsigned int )dat[0], FT_LOAD_RENDER );
    FT_GlyphSlot glyph = font->m_face->glyph;
    m_advanceX = glyph->advance.x >> 6;
    m_advanceY = glyph->advance.y >> 6;
    m_bitmapWidth = glyph->bitmap.width;
    m_bitmapHeight = glyph->bitmap.rows;
    m_bitmapLeft = glyph->bitmap_left;
    m_bitmapTop = glyph->bitmap_top;
    // If there's nothing to render, just return.
    if (glyph->bitmap.width <= 0 || glyph->bitmap.rows <= 0)
    {
        return;
    }
    // Insert image into texture atlas.
    is::TextureAtlas::Node* node = texture->insert( glyph->bitmap.width, glyph->bitmap.rows, glyph->bitmap.buffer );
    if ( !node ) {
        os->printf( "ERR Texture atlas overfilled!\n" );
        return;
    }
    m_renderable = true;
    // Now generate uv coords.
    float left = node->m_rect.left / texture->m_width;
    float width = node->m_rect.width / texture->m_width;
    float top = node->m_rect.top / texture->m_height;
    float height = node->m_rect.height / texture->m_height;
    m_uv[0] = glm::vec2( left, top );
    m_uv[1] = glm::vec2( left + width, top );
    m_uv[2] = glm::vec2( left + width, top - height );
    m_uv[3] = glm::vec2( left, top - height );
}
