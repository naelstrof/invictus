#include "icon.hpp"

is::Icon::Icon( std::string texturename ) {
    m_changed = true;
    m_texture = textures->get( texturename );
    glGenBuffers( 2, m_buffers);
    m_shader = shaders->get( "gui" );
}

is::Icon::~Icon() {
    glDeleteBuffers( 2, m_buffers );
}

void is::Icon::remove() {
    delete (is::Icon*)this;
}

std::string is::Icon::type() {
    return "icon";
}

void is::Icon::generateBuffers() {
    // We need to check to make sure we actually need to regenerate the buffers.
    if ( !m_changed ) {
        return;
    }

    std::vector<glm::vec2> verts;
    std::vector<glm::vec2> uvs;

    verts.push_back( glm::vec2( -0.5, -0.5 ) );
    verts.push_back( glm::vec2( 0.5, -0.5 ) );
    verts.push_back( glm::vec2( 0.5, 0.5 ) );
    verts.push_back( glm::vec2( -0.5, 0.5 ) );

    uvs.push_back( glm::vec2( 0, 1 ) );
    uvs.push_back( glm::vec2( 1, 1 ) );
    uvs.push_back( glm::vec2( 1, 0 ) );
    uvs.push_back( glm::vec2( 0, 0 ) );

    // Send the buffers to opengl and clean up
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[0] );
    glBufferData( GL_ARRAY_BUFFER, verts.size() * sizeof( glm::vec2 ), &verts[0], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, m_buffers[1] );
    glBufferData( GL_ARRAY_BUFFER, uvs.size() * sizeof( glm::vec2 ), &uvs[0], GL_STATIC_DRAW );
    m_vertcount = verts.size();
    m_changed = false;
}

void is::Icon::tick( float dt ) {
    // Make sure our texture is animating properly.
    m_texture->tick( dt );
    // SHAKE
    setPos( getPos() + ( dt*glm::vec3( float( rand()%300 ) - float( rand()%300 ),
                                  float( rand()%300 ) - float( rand()%300 ),
                                  float( rand()%300 ) - float( rand()%300 ) ) ) );
}

void is::Icon::draw() {
    // Here we'll make sure we have a properly generated buffer.
    generateBuffers();

    m_shader->bind();
    m_shader->setParameter( "texture", 0 );
    m_shader->setParameter( "color", getColor() );
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

void is::Icon::play( std::string name ) {
    m_texture->play( name );
}

bool is::Icon::visible() {
    // Always draw if part of the gui (due to the view frustum culling only working on orthographic objects)
    if ( m_shader->m_name == "gui" ) {
        return true;
    }
    // Otherwise test if we're inside the view frustum
    glm::vec3 size = getScale();
    m_hullsize = std::max( size.x, size.y )/2.f;

    is::Camera* cam = render->m_camera;
    //if ( cam->m_frustum->pointInFrustum( getPos(), &m_depth ) == is::Frustum::outside ) {
    if ( cam->m_frustum->sphereInFrustum( getPos(), m_hullsize, &m_depth ) == is::Frustum::outside ) {
        return false;
    }
    return true;
}
