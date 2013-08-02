#include "render.hpp"

is::Render* render = new is::Render();

is::Render::Render() {
    m_camera = new is::Camera();
    m_orthoMatrix = glm::mat4(1);
    m_perspMatrix = glm::mat4(1);
}

int is::Render::init() {
    os->printf( "INF OpenGL Vender: %\n", glGetString(GL_VENDOR) );
    os->printf( "INF OpenGL Version: %\n", glGetString(GL_VERSION) );
    os->printf( "INF OpenGL Renderer: %\n", glGetString(GL_RENDERER) );
    os->printf( "INF GLSL Version: %\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    return 0;
}

void is::Render::tick() {
    // Update our framebuffer (it won't recreate itself unless it has to).
    m_buffer.create( window->getWidth(), window->getHeight(), is::Framebuffer::color );

    // Grab all entities within 5000 units of the view fulstrum.
    m_camera->m_far = 5000;
    m_drawables = world->getSortedDrawables();
    if ( m_drawables.size() != 0 ) {
        // Here we set up the camera.
        m_camera->setRatio( window->getWidth(), window->getHeight() );
        m_camera->m_near = m_drawables.at(0)->m_depth-m_drawables.back()->m_hullsize;
        // Clamp above 0
        if ( m_camera->m_near <= 0 ) {
            m_camera->m_near = 0.1;
        }
        // +1 just so near and far can never equal.
        m_camera->m_far = m_drawables.back()->m_depth+m_drawables.back()->m_hullsize;
    }

    // Update the frustum
    m_camera->m_frustum->setCameraInternals( m_camera );

    // Set up perspective and ortho matricies. (Should happen every frame to make depth buffer accurate, orthographic ignores near plane limitations due to being 2D.)
    m_orthoMatrix = glm::ortho( 0.f, (float)window->getWidth(), 0.f, (float)window->getHeight(), 0.f, m_camera->m_far );
    m_perspMatrix = glm::perspective( m_camera->m_fov, float( window->getWidth() )/float( window->getHeight() ), m_camera->m_near, m_camera->m_far );
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
    window->setActive();
    glClearColor(1,1,1,1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  );

    m_buffer.bind();
    m_buffer.clear( glm::vec4(0,0,0,1) );
    for ( unsigned int i=0; i<m_drawables.size(); i++ ) {
        m_drawables[i]->draw();
    }
    m_buffer.unbind();
    m_drawables.clear();

    window->setActive();
    m_buffer.draw();
    gui->draw();
    window->display();
}
