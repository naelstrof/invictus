#include "window.hpp"

is::Window* window = new is::Window();

is::Window::Window() {
    m_window = NULL;
    // Focus should default to true, otherwise keyboard might get stuck disabled.
    m_focus = true;
}

is::Window::~Window() {
    if (!m_window) {
        return;
    }
    delete m_window;
}

int is::Window::init() {
    m_window = new sf::Window( sf::VideoMode( 512, 512 ), "Invictus" );
    return 0;
}

void is::Window::tick( float dt ) {
    if ( !m_window->isOpen() ) {
        return;
    }
    sf::Event event;
    while ( m_window->pollEvent( event ) ) {
        switch ( event.type ) {
            case sf::Event::Closed: {
                m_window->close();
                break;
            }
            case sf::Event::Resized: {}
            case sf::Event::LostFocus: {
                m_focus = false;
                break;
            }
            case sf::Event::GainedFocus: {
                m_focus = true;
                break;
            }
            case sf::Event::TextEntered: {
                keyboard->addInput( event.text.unicode );
                break;
            }
            case sf::Event::KeyPressed: {}
            case sf::Event::KeyReleased: {}
            case sf::Event::MouseWheelMoved: {}
            case sf::Event::MouseButtonPressed: {}
            case sf::Event::MouseButtonReleased: {}
            case sf::Event::MouseMoved: {}
            case sf::Event::MouseEntered: {}
            case sf::Event::MouseLeft: {}
            case sf::Event::JoystickButtonPressed: {}
            case sf::Event::JoystickButtonReleased: {}
            case sf::Event::JoystickMoved: {}
            case sf::Event::JoystickConnected: {}
            case sf::Event::JoystickDisconnected: {}
            case sf::Event::Count: {}
        }
    }
}

bool is::Window::isOpen() {
    return m_window->isOpen();
}

bool is::Window::isFocused() {
    return m_focus;
}

void is::Window::swapBuffers() {
    m_window->display();
}

void is::Window::close() {
    m_window->close();
}
