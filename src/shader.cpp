#include "render.hpp"

is::Render* render = new is::Render();

is::Render::Render() {
}

int is::Render::init() {
    // Create an FBO for the world and the gui.
    // Simply so we can apply post processing effects on each separately.
    m_world.create( window->getWidth(), window->getHeight(), true );
    m_gui.create( window->getWidth(), window->getHeight(), true );
    return 0;
}

void is::Render::draw() {
    // Render to the world FBO.
    m_world.setActive();
    m_world.clear();
    world->draw();
    m_world.display();

    // Now render to the gui FBO.
    m_gui.setActive();
    m_gui.clear();
    gui->draw();
    m_gui.display();

    // Now render both to screen.
    window->setActive();
    window->clear();
    window->draw( &m_world );
    window->draw( &m_gui );
    window->display();
}