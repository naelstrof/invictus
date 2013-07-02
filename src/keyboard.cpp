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
    // Gotta convert to utf-8 so input can be printed or used whereever.
    std::string test;
    sf::Utf<32>::toUtf8( m_input.begin(), m_input.end(), back_inserter( test ) );
    clearInput();
    return test;
}

void is::Keyboard::addInput( sf::String input ) {
    // We don't want to use too much memory tracking generated characters, once we overflow we just stop accepting characters.
    if ( m_input.getSize() > 512 ) {
        return;
    }
    m_input += input;
}
