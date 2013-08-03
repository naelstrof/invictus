#include "mouse.hpp"

is::Mouse* mouse = new is::Mouse();

is::Mouse::Mouse() {
    m_memory = sf::Vector2i(0,0);
}

bool is::Mouse::isDown( is::Mouse::Button button ) {
    // Since sfml's mouse doesn't care if the window is in focus, we manually check if the window is in focus.
    if ( !window->isFocused() ) {
        return false;
    }
    return sf::Mouse::isButtonPressed( (sf::Mouse::Button) button );
}

glm::vec3 is::Mouse::getPos() {
    // Don't update mouse position if we're not in focus.
    if ( !window->isFocused() ) {
        return glm::vec3( m_memory.x, m_memory.y, 0 );
    }
    m_memory = sf::Mouse::getPosition( *window->m_window );
    return glm::vec3( m_memory.x, m_memory.y, 0 );
}

void is::Mouse::setPos( glm::vec3 pos ) {
    // We allow the setting of memory, so if the player alt-tabs while turning in-game. The turning will stop and further input will stop.
    m_memory = sf::Vector2i( pos.x, pos.y );
    if ( !window->isFocused() ) {
        return;
    }
    sf::Mouse::setPosition( m_memory, *window->m_window );
}
