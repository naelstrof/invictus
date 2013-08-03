// keyboard.hpp: Allows for grabbing keyboard states.

#ifndef IS_KEYBOARD_H_
#define IS_KEYBOARD_H_

#include "window.hpp"

namespace is {

class Keyboard {
private:
    sf::String      m_input;
public:

// Keysyms were put in another file for cleanliness.
#include "keysym.hpp"

    bool            isDown( is::Keyboard::Key key );
    void            clearInput();
    std::string     getInput();
    void            addInput( sf::String input );
};

};

extern is::Keyboard* keyboard;

#endif // IS_KEYBOARD_H_
