#include "text.hpp"

is::Text::Text( sf::String text, std::string fontname, int size )
    : m_text( text ), m_font( fontname ), m_size( size ), m_changed( true ),
      m_texture( glyphs->getTexture( fontname, size ) ), m_vertcount( 0 ) {

    if ( !m_texture ) {
        os->printf( "ERR Font not found: %\n", fontname );
    }
    m_textureSize = m_texture->m_size;
    glGenBuffers( 2, m_buffers);
    m_shader = shaders->get( "text" );
    m_width = 0;
    m_height = 0;
    m_renderMode = is::Text::Middle;
}

is::Text::~Text() {
    glDeleteBuffers( 2, m_buffers);
}

void is::Text::setText( sf::String text ) {
    if ( text == m_text ) {
        return;
    }
    m_text = text;
    getDimensions();
    m_changed = true;
}

std::string is::Text::getText() {
    std::string text;
    sf::Utf<32>::toUtf8( m_text.begin(), m_text.end(), text.begin() );
    return text;
}

void is::Text::remove() {
    delete (is::Text*)this;
}

std::string is::Text::type() {
    return "text";
}

void is::Text::getDimensions() {
    if ( !m_changed && ( m_width || m_height ) ) {
        return;
    }
    m_width = 0;
    m_height = 0;
    float penx = 0;
    for ( unsigned int i=0; i<m_text.getSize(); i++ ) {
        is::Glyph* glyph = glyphs->get( m_text[i], m_font, m_size );
        if ( !glyph ) {
            continue;
        }
        if ( m_text[i] == (unsigned int)'\n' ) {
            m_width = std::max( m_width, penx+5 );
            penx = 0;
            continue;
        }
        // The shadow size is a static 5 in the shader as well as here and in glyphs->get()
        float h = glyph->m_bitmapHeight-5;
        penx += glyph->m_advanceX;
        m_height = std::max( m_height, h );
    }
    m_width = std::max( m_width, penx+5 );
}

void is::Text::generateBuffers() {
    // We need to check to make sure we actually need to regenerate the buffers.
    if ( !m_changed && m_textureSize == m_texture->m_size ) {
        return;
    }
    m_textureSize = m_texture->m_size;
    float penx = 0;
    float peny = 0;
    getDimensions();
    std::vector<glm::vec2>  uvs;
    std::vector<glm::vec2>  verts;
    for ( unsigned int i=0; i<m_text.getSize(); i++ ) {
        // Generate glyph information and render it to a texture atlas.
        is::Glyph* glyph = glyphs->get( m_text[i], m_font, m_size );
        if ( !glyph ) {
            continue;
        }
        if ( m_text[i] == (unsigned int)'\n' ) {
            penx = 0;
            peny -= m_size;
            continue;
        }
        // The shadow size is a static 5 in the shader as well as here and in glyphs->get()
        float shadowSize = 5;
        float w = glyph->m_bitmapWidth+shadowSize;
        float h = glyph->m_bitmapHeight+shadowSize;
        float xoff = 0;
        float yoff = 0;
        switch ( m_renderMode ) {
            case is::Text::Left: {
                xoff = glyph->m_bitmapLeft;
                yoff = glyph->m_bitmapTop-float( m_height ) / 2.f;
                break;
            }
            case is::Text::Middle: {
                xoff = glyph->m_bitmapLeft-float( m_width ) / 2.f;
                yoff = glyph->m_bitmapTop-float( m_height ) / 2.f;
                break;
            }
            case is::Text::Right: {
                xoff = glyph->m_bitmapLeft-float( m_width );
                yoff = glyph->m_bitmapTop-float( m_height ) / 2.f;
                break;
            }
            default: break;
        }
        verts.push_back( glm::vec2( penx+xoff,    peny+yoff-h ) );
        verts.push_back( glm::vec2( penx+xoff+w,  peny+yoff-h ) );
        verts.push_back( glm::vec2( penx+xoff+w,  peny+yoff ) );
        verts.push_back( glm::vec2( penx+xoff,    peny+yoff ) );

        uvs.push_back( glyph->m_uv[0] );
        uvs.push_back( glyph->m_uv[1] );
        uvs.push_back( glyph->m_uv[2] );
        uvs.push_back( glyph->m_uv[3] );

        penx += glyph->m_advanceX;
        peny += glyph->m_advanceY;
    }
    // Send the buffers to opengl and clean up
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[0] );
    glBufferData( GL_ARRAY_BUFFER, verts.size() * sizeof( glm::vec2 ), &verts[0], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[1] );
    glBufferData( GL_ARRAY_BUFFER, uvs.size() * sizeof( glm::vec2 ), &uvs[0], GL_STATIC_DRAW );
    m_vertcount = verts.size();
    // We need to check if the texture atlas resized itself during glyph rendering, if it did we need to regenerate the buffers again...
    if ( m_texture->changed() ) {
        generateBuffers();
    }
    m_changed = false;
}


void is::Text::tick( float dt ) {
    //SPIN
    //setAng(getAng()+glm::vec3(dt,0,0));
    //setSize( 34+fabs(sin(m_totaltime))*24);
}

void is::Text::draw() {
    // Here we'll make sure we have a properly generated buffer.
    generateBuffers();

    m_shader->bind();
    m_shader->setParameter( "texture", 0 );
    m_shader->setParameter( "color", getColor() );
    m_shader->setParameter( "textureSize", glm::vec2( (float)m_texture->m_size ) );
    m_shader->setParameter( "model", getModelMatrix() );
    m_shader->setAttribute( "vertex", m_buffers[0], 2 );
    m_shader->setAttribute( "uv", m_buffers[1], 2 );

    m_texture->bind();
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_TEXTURE_2D );
    glDrawArrays( GL_QUADS, 0, m_vertcount );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );

    m_shader->unbind();
}

void is::Text::setSize( unsigned int size ) {
    if ( m_size == size ) {
        return;
    }
    m_size = size;
    m_changed = true;
    m_texture = glyphs->getTexture( m_font, size );
}

bool is::Text::visible() {
    // We'll assume all 2D orthographic text is visible unless otherwise specified.
    return true;
}
