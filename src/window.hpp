// window.hpp: Handles all window events and interactions, includes sending key events to keyboard.

#ifndef IS_WINDOW_H_
#define IS_WINDOW_H_

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "keyboard.hpp"
#include "lua.hpp"

namespace is {

class Window {
private:
    sf::RenderWindow*   m_window;
    bool                m_focus;
    bool                m_fullscreen;
    bool                m_noborder;
    bool                m_vsync;
    int                 m_maxfps;
    int                 m_width;
    int                 m_height;
    std::string         m_name;
    bool                m_printed;
    bool                m_changed;
public:
    Window();
    ~Window();
    void                tick();
    int                 init();
    void                display();
    void                close();
    bool                isOpen();
    bool                isFocused();
    void                setSize( int w, int h );
    int                 getWidth();
    int                 getHeight();
    void                printModes();
    void                setActive();
    void                setNoBorder( bool foo );
    void                setFullscreen( bool foo );
    void                clear();
    void                draw( sf::RenderTexture* foo );
};

};

extern is::Window* window;

#endif // IS_WINDOW_H_
