#include "text.hpp"

is::Text::Text( sf::String text, std::string fontname, int size ) {
    m_text = text;
    m_font = fontname;
    m_size = size;
    m_changed = true;
    m_texture = glyphs->getTexture( fontname );
}

is::Text::~Text() {

}

void is::Text::remove() {
    delete (is::Text*)this;
}

std::string is::Text::type() {
    return "text";
}

void is::Text::tick( float dt ) {
    // Here we'll make sure we have a properly generated buffer.
    if ( !m_changed ) {
        return;
    }
    float penx = 0;
    float peny = 0;
    for ( unsigned int i=0; i<m_text.size(); i++ ) {
        // Generate glyph information and render it to a texture atlas.
        is::GlyphInfo* glyph = glyphs->get( m_text[i], m_font, m_size );
        if ( !glyph ) {
            continue;
        }
        float w = glyph->m_bitmapHeight;
        float w = glyph->m_bitmapWidth;
        float xoff = glyph->m_bitmapLeft;
        float yoff = glyph->m_bitmapTop;
        m_verts.push_back( glm::vec2( penx+xoff,    peny-yoff+h ) );
        m_verts.push_back( glm::vec2( penx+xoff+w,  peny-yoff+h ) );
        m_verts.push_back( glm::vec2( penx+xoff+w,  peny-yoff ) );
        m_verts.push_back( glm::vec2( penx+xoff,    peny-yoff ) );

        m_uvs.push_back( glyphs->m_uv[0] );
        m_uvs.push_back( glyphs->m_uv[1] );
        m_uvs.push_back( glyphs->m_uv[2] );
        m_uvs.push_back( glyphs->m_uv[3] );

        penx += glyph->m_advanceX;
        peny += glyph->m_advanceY;
    }
    // Send the buffers to opengl and clean up
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[0]);
    glBufferData( GL_ARRAY_BUFFER, m_verts.size() * sizeof( glm::vec2 ), &m_verts[0], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[1]);
    glBufferData( GL_ARRAY_BUFFER, m_uvs.size() * sizeof( glm::vec2 ), &m_uvs[0], GL_STATIC_DRAW );
    m_verts.clear();
    m_uvs.clear();
}

void is::Text::draw( sf::RenderTarget* target ) {
    sf::Shader* shader = shaders->get( "flat" );
    shader->bind();
    m_texture->bind();
    shader->setParameter( "texture", 0 );
    shader->setParameter( "texture", 0 );

}
