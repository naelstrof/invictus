#include "text.hpp"

is::Text::Text( sf::String text, std::string fontname, int size )
    : m_text( text ), m_font( fontname ), m_size( size ), m_changed( true ),
      m_texture( glyphs->getTexture( fontname, size ) ), m_vertcount( 0 ), m_totaltime( 0 ) {

    if ( !m_texture ) {
        os->printf( "ERR Font not found: %\n", fontname );
    }
    m_textureSize = m_texture->m_size;
    glGenBuffers( 2, m_buffers);
}

is::Text::~Text() {
    glDeleteBuffers( 2, m_buffers);
}

void is::Text::remove() {
    delete (is::Text*)this;
}

std::string is::Text::type() {
    return "text";
}

void is::Text::generateBuffers() {
    // We need to check to make sure we actually need to regenerate the buffers.
    if ( !m_changed && m_textureSize == m_texture->m_size ) {
        return;
    }
    m_textureSize = m_texture->m_size;
    float penx = 0;
    float peny = 0;
    for ( unsigned int i=0; i<m_text.getSize(); i++ ) {
        //std::string text;
        //sf::Utf<32>::toUtf8( m_text.begin()+i, m_text.begin()+i+1, back_inserter( text ) );
        //os->printf( "%: %\n", text, m_text[i] );
        // Generate glyph information and render it to a texture atlas.
        is::Glyph* glyph = glyphs->get( m_text[i], m_font, m_size );
        if ( !glyph ) {
            continue;
        }
        // The shadow size is a static 5 in the shader as well as here and in glyphs->get()
        float shadowSize = 5;
        float w = glyph->m_bitmapWidth+shadowSize;
        float h = glyph->m_bitmapHeight+shadowSize;
        float xoff = glyph->m_bitmapLeft;
        float yoff = glyph->m_bitmapTop;
        m_verts.push_back( glm::vec2( penx+xoff,    peny+yoff-h ) );
        m_verts.push_back( glm::vec2( penx+xoff+w,  peny+yoff-h ) );
        m_verts.push_back( glm::vec2( penx+xoff+w,  peny+yoff ) );
        m_verts.push_back( glm::vec2( penx+xoff,    peny+yoff ) );

        m_uvs.push_back( glyph->m_uv[0] );
        m_uvs.push_back( glyph->m_uv[1] );
        m_uvs.push_back( glyph->m_uv[2] );
        m_uvs.push_back( glyph->m_uv[3] );

        penx += glyph->m_advanceX;
        //peny += glyph->m_advanceY;
    }
    // Send the buffers to opengl and clean up
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[0] );
    glBufferData( GL_ARRAY_BUFFER, m_verts.size() * sizeof( glm::vec2 ), &m_verts[0], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[1] );
    glBufferData( GL_ARRAY_BUFFER, m_uvs.size() * sizeof( glm::vec2 ), &m_uvs[0], GL_STATIC_DRAW );
    m_vertcount = m_verts.size();
    m_verts.clear();
    m_uvs.clear();
    // We need to check if the texture atlas resized itself during glyph rendering, if it did we need to regenerate the buffers again...
    if ( m_texture->changed() ) {
        generateBuffers();
    }
    m_changed = false;
}


void is::Text::tick( float dt ) {
    //SPIN
    //setAng(getAng()+glm::vec3(dt,0,0));
    setSize( fabs(sin(m_totaltime))*128 + 1 );
    m_totaltime += dt;
}

void is::Text::draw() {
    // Here we'll make sure we have a properly generated buffer.
    generateBuffers();

    is::Shader* shader = shaders->get( "text" );
    shader->bind();
    shader->setParameter( "texture", 0 );
    shader->setParameter( "color", getColor() );
    shader->setParameter( "model", getModelMatrix() );
    shader->setAttribute( "vertex", m_buffers[0], 2 );
    shader->setAttribute( "uv", m_buffers[1], 2 );

    m_texture->bind();
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_TEXTURE_2D );
    glDrawArrays( GL_QUADS, 0, m_vertcount );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );

    shader->unbind();
}

void is::Text::setSize( unsigned int size ) {
    if ( m_size == size ) {
        return;
    }
    m_size = size;
    m_changed = true;
    m_texture = glyphs->getTexture( m_font, size );
}
