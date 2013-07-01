// window.hpp: Handles all window events and interactions, includes sending key events to keyboard.

#ifndef IS_WINDOW_H_
#define IS_WINDOW_H_

#include <SFML/Window.hpp>
// Required to convert utf-32 into utf-8.
#include <boost/nowide/args.hpp>

#include "keyboard.hpp"

namespace is {

class Window {
private:
    sf::Window*         m_window;
    bool                m_focus;
public:
    Window();
    ~Window();
    void                tick( float dt );
    int                 init();
    void                swapBuffers();
    void                close();
    bool                isOpen();
    bool                isFocused();
};

};

extern is::Window* window;

#endif // IS_WINDOW_H_
