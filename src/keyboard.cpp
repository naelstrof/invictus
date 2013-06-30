#include "keyboard.hpp"

is::Keyboard* keyboard = new is::Keyboard();

bool is::Keyboard::isDown( is::Key key ) {
    // Since sfml's keyboard doesn't care if the window is in focus, we manually check if the window is in focus.
    if ( !window->isFocused() ) {
        return false;
    }
    return sf::Keyboard::isKeyPressed( (sf::Keyboard::Key) key );
}

void is::Keyboard::clearInput() {
    m_input.clear();
}

std::string is::Keyboard::getInput() {
    std::string copy = m_input;
    clearInput();
    return m_input;
}

void is::Keyboard::addInput( std::string input ) {
    // We don't want to use too much memory tracking generated characters, once we overflow we just stop accepting characters.
    if ( m_input.length() > 512 ) {
        return;
    }
    m_input += input;
}
