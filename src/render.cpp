#include "render.hpp"

is::Render* render = new is::Render();

is::Render::Render() {
    m_sizeChanged = false;
}

int is::Render::init() {
    // Create an FBO for the world and the gui.
    // Simply so we can apply post processing effects on each separately.
    m_world.create( window->getWidth(), window->getHeight(), true );
    m_gui.create( window->getWidth(), window->getHeight(), false );

    glDisable( GL_CULL_FACE );
    return 0;
}

void is::Render::tick() {
    GLenum glErr = glGetError();
    if (glErr != GL_NO_ERROR )
    {
        os->printf( "ERR OpenGL error: %\n", gluErrorString( glErr ) );
    }
}

void is::Render::draw() {
    if ( m_sizeChanged ) {
        init();
        m_sizeChanged = false;
    }
    // Render to the world FBO.
    /*m_world.setActive();
    m_world.clear();
    //m_world.clear(sf::Color(1,1,1,1));
    world->draw( &m_world );
    m_world.display();

    // Now render to the gui FBO.
    m_gui.setActive();
    m_gui.clear();
    //m_gui.clear(sf::Color(1,1,1,1));
    gui->draw( &m_gui );
    m_gui.display();*/

    window->setActive();
    glClearColor(1,1,1,1);
    glClear( GL_COLOR_BUFFER_BIT );
    //window->draw( &m_world );
    //window->draw( &m_gui );
    gui->draw( NULL );
    window->display();
}

void is::Render::updateSize() {
    m_sizeChanged = true;
}
