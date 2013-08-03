// mouse.hpp: Allows for grabbing mouse states.

#ifndef IS_MOUSE_H_
#define IS_MOUSE_H_

#include <SFML/Window/Mouse.hpp>

#include "window.hpp"

namespace is {

class Mouse {
private:
    sf::Vector2i    m_memory;
public:
    Mouse();
    enum Button {
        Left,
        Right,
        Middle,
        XButton1,
        XButton2,
        ButtonCount
    };
    bool            isDown( is::Mouse::Button button );
    glm::vec3       getPos();
    void            setPos( glm::vec3 );
};

};

extern is::Mouse* mouse;

#endif // IS_MOUSE_H_
