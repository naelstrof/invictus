#include "render.hpp"

is::Render* render = new is::Render();

is::Render::Render() {
    m_camera = NULL;
    m_orthoMatrix = glm::mat4(1);
    m_perspMatrix = glm::mat4(1);
}

int is::Render::init() {
    glEnable( GL_CULL_FACE );
    return 0;
}

void is::Render::tick() {
    m_guiBuffer.create( window->getWidth(), window->getHeight(), is::Framebuffer::color | is::Framebuffer::depth );
    // Create the camera if it doesn't exist.
    if ( m_camera == NULL ) {
        m_camera = new Camera();
        world->addNode( m_camera );
    }
    // Set up perspective and ortho matricies. (Should happen every frame to make depth buffer accurate)
    m_orthoMatrix = glm::ortho( 0.f, (float)window->getWidth(), 0.f, (float)window->getHeight(), 0.f, 3000.f );
    m_perspMatrix = glm::perspective( 45.f, float( window->getWidth() )/float( window->getHeight() ), 0.f, 3000.f );
    // Loop through all shaders, setting up View and World matricies.
    std::vector<is::Shader*> shaderlist = shaders->getAll();
    for ( unsigned int i=0; i<shaderlist.size(); i++ ) {
        is::Shader* shader = shaderlist[i];
        switch ( shader->m_type ) {
            // Perspective
            case 0: {
                shader->setParameter( "view", m_camera->getViewMatrix() );
                shader->setParameter( "world", m_perspMatrix );
                break;
            }
            // Orthographic
            case 1: {
                shader->setParameter( "world", m_orthoMatrix );
                break;
            }
            // Everything else
            default: {
                break;
            }
        }
    }
    GLenum glErr = glGetError();
    if (glErr != GL_NO_ERROR )
    {
        os->printf( "ERR OpenGL error: %\n", gluErrorString( glErr ) );
    }
}

void is::Render::draw() {

    m_guiBuffer.bind();
    m_guiBuffer.clear( glm::vec4(0,0,0,1) );
    gui->draw();
    m_guiBuffer.unbind();

    window->setActive();
    glClearColor(1,1,1,1);
    glClear( GL_COLOR_BUFFER_BIT );
    m_guiBuffer.draw();
    //window->draw( &m_world );
    //window->draw( &m_gui );
    window->display();
}
