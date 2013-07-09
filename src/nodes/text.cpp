#include "text.hpp"

is::Text::Text( sf::String text, std::string fontname, int size ) {
    m_text = text;
    m_font = fontname;
    m_size = size;
    m_changed = true;
    m_texture = glyphs->getTexture( fontname );
    if ( !m_texture ) {
        os->printf( "ERR Font not found: %\n", fontname );
    }
    m_vertcount = 0;
    glGenBuffers( 2, m_buffers);
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
    for ( unsigned int i=0; i<m_text.getSize(); i++ ) {
        // Generate glyph information and render it to a texture atlas.
        is::Glyph* glyph = glyphs->get( m_text[i], m_font, m_size );
        if ( !glyph ) {
            continue;
        }
        float w = glyph->m_bitmapWidth;
        float h = glyph->m_bitmapHeight;
        float xoff = glyph->m_bitmapLeft;
        float yoff = glyph->m_bitmapTop;
        m_verts.push_back( glm::vec2( penx+xoff,    peny+yoff ) );
        m_verts.push_back( glm::vec2( penx+xoff+w,  peny+yoff ) );
        m_verts.push_back( glm::vec2( penx+xoff+w,  peny+yoff-h ) );
        m_verts.push_back( glm::vec2( penx+xoff,    peny+yoff-h ) );

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
}

void is::Text::draw( sf::RenderTarget* target ) {
    is::Shader* shader = shaders->get( "text" );
    unsigned int program = shader->getProgram();
    shader->bind();
    glActiveTexture( GL_TEXTURE0 );
    m_texture->bind();
    shader->setParameter( "texture", 0 );
    shader->setParameter( "matrix", getMatrix() );
    shader->setParameter( "projection", glm::ortho( 0.f, (float)window->getWidth(), 0.f, (float)window->getHeight(), 0.f, 1.f ) );
    //shader->setParameter( "view", glm::mat4() );

    unsigned int va = glGetAttribLocation( program, "vertex" );
    glEnableVertexAttribArray( va );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[0] );
    glVertexAttribPointer( va, 2, GL_FLOAT, GL_FALSE, 0, NULL );

    unsigned int uv = glGetAttribLocation( program, "uv" );
    glEnableVertexAttribArray( uv );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[1] );
    glVertexAttribPointer( uv, 2, GL_FLOAT, GL_FALSE, 0, NULL );

    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_TEXTURE_2D );
    glDrawArrays( GL_QUADS, 0, m_vertcount );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );

    glDisableVertexAttribArray( va );
    glDisableVertexAttribArray( uv );

    glUseProgram( 0 );
}
