#include "glyph.hpp"

is::GlyphLoader* glyphs = new is::GlyphLoader();

is::GlyphLoader::~GlyphLoader() {
    for ( unsigned int i=0; i<m_glyphcontainers.size(); i++ ) {
        delete m_glyphcontainers[i];
    }
}

is::TextureAtlas* is::GlyphLoader::getTexture( std::string fontname ) {
    // Get texture starting size.
    m_textureStartSize = 512;
    float check = lua->getFloat( "textureAtlasStartSize" );
    if ( check > 0 ) {
        m_textureStartSize = ( unsigned int )check;
    }

    for ( unsigned int i=0; i<m_glyphcontainers.size(); i++ ) {
        if ( fontname == m_glyphcontainers[i]->m_font->m_name ) {
            return m_glyphcontainers[i]->m_texture;
        }
    }
    // Try to create the texture if it doesn't exist.
    get( "a", fontname, 16 );
    for ( unsigned int i=0; i<m_glyphcontainers.size(); i++ ) {
        if ( fontname == m_glyphcontainers[i]->m_font->m_name ) {
            return m_glyphcontainers[i]->m_texture;
        }
    }
    return NULL;
}

is::Glyph* is::GlyphLoader::get( sf::String id, std::string fontname, int size ) {
    // Make sure the string is only one character long
    if ( id.getSize() > 1 ) {
        os->printf( "ERR Unexpected call to is::GlyphLoader::get() with id containing more than one character!\n" );
        return NULL;
    }
    // Then search if we can find the specified font.
    for ( unsigned int i=0; i<m_glyphcontainers.size(); i++ ) {
        if ( fontname == m_glyphcontainers[i]->m_font->m_name ) {
            is::Glyph* info = m_glyphcontainers[i]->find( id, size );
            if ( !info ) {
                std::string text;
                sf::Utf<32>::toUtf8( id.begin(), id.end(), back_inserter( text ) );
                os->printf( "ERR The glyph % cannot be found!", text );
                return NULL;
            }
            // Here we check if the texture size has recently changed, and if so fix the glyph.
            info->fixUV( m_glyphcontainers[i]->m_texture );
            return info;
        }
    }
    // Now since the font doesn't exist, we must grab it and initialize it.
    is::Font* font = fonts->get( fontname );
    if ( !font ) {
        os->printf( "ERR The font % cannot be found!", fontname );
        return NULL;
    }
    m_glyphcontainers.push_back( new is::GlyphsContainer( font, m_textureStartSize ) );
    return m_glyphcontainers.back()->find( id, size );
}

is::GlyphsContainer::~GlyphsContainer() {
    delete m_texture;
}

is::GlyphsContainer::GlyphsContainer( is::Font* font, int textureSize ) {
    m_font = font;
    m_texture = new is::TextureAtlas( textureSize, textureSize );
}

is::Glyph* is::GlyphsContainer::add( sf::String id, int size ) {
    m_glyphs.push_back( is::Glyph( m_font, size, id, m_texture ) );
    return &( m_glyphs.back() );
}

is::Glyph* is::GlyphsContainer::find( sf::String id, int size ) {
    for ( unsigned int i=0; i<m_glyphs.size(); i++ ) {
        if ( m_glyphs[i].m_id == id && m_glyphs[i].m_size == size ) {
            return &( m_glyphs[i] );
        }
    }
    return add( id, size );
}

is::Glyph::Glyph( is::Font* font, int size, sf::String id, is::TextureAtlas* texture ) {
    m_texturesize = 0;
    m_renderable = false;
    m_size = size;
    m_id = id;
    // Set the height of the glyph in pixels.
    FT_Set_Pixel_Sizes( font->m_face, 0, m_size );
    const sf::Uint32* dat = id.getData();
    FT_Load_Char( font->m_face, ( unsigned long long )dat[0], FT_LOAD_RENDER );
    FT_GlyphSlot glyph = font->m_face->glyph;
    m_advanceX = glyph->advance.x >> 6;
    m_advanceY = glyph->advance.y >> 6;
    m_bitmapWidth = glyph->bitmap.width;
    m_bitmapHeight = glyph->bitmap.rows;
    m_bitmapLeft = glyph->bitmap_left;
    m_bitmapTop = glyph->bitmap_top;
    // If there's nothing to render, just return.
    //if (glyph->bitmap.width <= 0 || glyph->bitmap.rows <= 0)
    //{
        //return;
    //}
    // Insert image into texture atlas.
    m_node = texture->insert( glyph->bitmap.width, glyph->bitmap.rows, glyph->bitmap.buffer );
    m_renderable = true;
    // Now generate uv coords.
    fixUV( texture );
}

void is::Glyph::fixUV( is::TextureAtlas* texture ) {
    if ( m_texturesize == texture->m_width ) {
        return;
    }
    // Texture's size should always be square.
    m_texturesize = texture->m_width;
    float left = (float)m_node->m_rect.left / m_texturesize;
    float width = (float)m_node->m_rect.width / m_texturesize;
    float top = (float)m_node->m_rect.top / m_texturesize;
    float height = float(m_node->m_rect.height) / m_texturesize;
    m_uv[0] = glm::vec2( left, top-height );
    m_uv[1] = glm::vec2( left + width, top-height );
    m_uv[2] = glm::vec2( left + width, top );
    m_uv[3] = glm::vec2( left, top );
}
