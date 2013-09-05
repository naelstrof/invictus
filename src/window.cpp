#include "window.hpp"

is::Window* window = new is::Window();

is::Window::Window() {
    m_window = NULL;
    // Focus should default to true, otherwise keyboard might get stuck disabled.
    m_focus = true;
    m_fullscreen = false;
    m_noborder = false;
    m_vsync = false;
    m_maxfps = 300;
    m_width = 512;
    m_height = 512;
    m_printed = false;
    m_window = new sf::RenderWindow();
    m_changed = false;
}

is::Window::~Window() {
    if (!m_window) {
        return;
    }
    delete m_window;
}

// This completely handles window creation and updates.
int is::Window::init() {
    setIcon( "invictus.png" );
    // Get the configuration options for the window.
    m_fullscreen = lua->getBool( "fullscreen" );
    m_noborder = lua->getBool( "noBorder" );
    m_vsync = lua->getBool( "verticalSync" );
    m_width = int( lua->getFloat( "width" ) );
    m_height = int( lua->getFloat( "height" ) );
    m_name = lua->getString( "windowName" );
    m_maxfps = int( lua->getFloat( "maxFPS" ) );
    int mode = int( lua->getFloat( "videoMode" ) );
    if ( mode < 0 ) {
        m_mode = sf::VideoMode( m_width, m_height );
    } else if ( mode == 0 ) {
        m_mode = sf::VideoMode::getDesktopMode();
    } else {
        mode--;
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
        if ( mode < (int)modes.size() ) {
            m_mode = modes.at( mode );
        } else {
            m_mode = modes.back();
        }
    }

    printModes();

    if ( !m_fullscreen ) {
        if ( m_noborder ) {
            m_window->create( m_mode, m_name, sf::Style::None );
        } else {
            m_window->create( m_mode, m_name );
        }
        m_window->setFramerateLimit( m_maxfps );
        m_window->setVerticalSyncEnabled( m_vsync );
        return 0;
    }

    // If we're noborder'd and fullscreened, just use desktop res.
    if ( m_noborder && m_fullscreen ) {
        m_window->create( sf::VideoMode::getDesktopMode(), m_name, sf::Style::None );
        m_window->setFramerateLimit( m_maxfps );
        m_window->setVerticalSyncEnabled( m_vsync );
        return 0;
    }

    // If we're fullscreen, check if we have a valid video mode. Otherwise just use the desktop res.
    if ( !m_mode.isValid() ) {
        m_window->create( sf::VideoMode::getDesktopMode(), m_name, sf::Style::Fullscreen );
        os->printf( "WRN %x% is an invalid video mode!\n", m_width, m_height );
        os->printf( "INF Using desktop resolution instead.\n" );
        m_window->setFramerateLimit( m_maxfps );
        m_window->setVerticalSyncEnabled( m_vsync );
        return 0;
    }

    m_window->create( m_mode, m_name, sf::Style::Fullscreen );
    m_window->setFramerateLimit( m_maxfps );
    m_window->setVerticalSyncEnabled( m_vsync );
    return 0;
}

void is::Window::tick() {
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
            case sf::Event::Resized: {
                glViewport( 0, 0, event.size.width, event.size.height );
                break;
            }
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
            //case sf::Event::KeyPressed: {}
            //case sf::Event::KeyReleased: {}
            //case sf::Event::MouseWheelMoved: {}
            //case sf::Event::MouseButtonPressed: {}
            //case sf::Event::MouseButtonReleased: {}
            //case sf::Event::MouseMoved: {}
            //case sf::Event::MouseEntered: {}
            //case sf::Event::MouseLeft: {}
            //case sf::Event::JoystickButtonPressed: {}
            //case sf::Event::JoystickButtonReleased: {}
            //case sf::Event::JoystickMoved: {}
            //case sf::Event::JoystickConnected: {}
            //case sf::Event::JoystickDisconnected: {}
            //case sf::Event::Count: {}
            default: {
                break;
            }
        }
    }
    // If the window changed, update it.
    if ( m_changed ) {
        init();
        m_changed = false;
    }
}

void is::Window::setActive() {
    m_window->setActive();
}

bool is::Window::isOpen() {
    return m_window->isOpen();
}

bool is::Window::isFocused() {
    return m_focus;
}

void is::Window::display() {
    m_window->display();
}

void is::Window::close() {
    m_window->close();
}

void is::Window::draw( sf::RenderTexture* foo, const sf::RenderStates &states ) {
    sf::Sprite sprite( foo->getTexture() );
    m_window->draw( sprite, states );
}

void is::Window::draw( const sf::Drawable &drawable, const sf::RenderStates &states ) {
    m_window->draw( drawable, states );
}

void is::Window::setSize( int w, int h ) {
    sf::Vector2u csize = m_window->getSize();
    // If w or h are 0, don't change the size of that axis.
    m_window->setSize( sf::Vector2u( w ? w : csize.x, h ? h : csize.y ) );
}

int is::Window::getWidth() {
    return (int)m_window->getSize().x;
}

int is::Window::getHeight() {
    return (int)m_window->getSize().y;
}

void is::Window::printModes() {
    if ( m_printed ) {
        return;
    }
    m_printed = true;
    // Print available video modes, but only of the highest bitsPerPixels. Since there's like thousands of modes otherwise :x.
    os->printf( "INF Available Video modes: (Only showing highest bits per pixel)\n" );
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    unsigned int highestBits = 0;
    for ( unsigned int i=0;i<modes.size();i++ ) {
        if ( modes[i].bitsPerPixel > highestBits ) {
            highestBits = modes[i].bitsPerPixel;
        }
    }
    for ( unsigned int i=0;i<modes.size();i++ ) {
        if ( modes[i].bitsPerPixel == highestBits ) {
            os->printf( "INF     ID: %, Width: %, Height: %, Bits: %\n", i, modes[i].width, modes[i].height, modes[i].bitsPerPixel );
        }
    }
}

// Instead of recreating the window here, we just tell it to update as soon as possible. So many changes to the window doesn't cause it to recreate itself 30 times...
void is::Window::setNoBorder( bool foo ) {
    if ( m_noborder == foo ) {
        return;
    }
    lua->setBool( "noBorder", foo );
    m_noborder = foo;
    m_changed = true;
}

void is::Window::setFullscreen( bool foo ) {
    if ( m_noborder == foo ) {
        return;
    }
    lua->setBool( "fullscreen", foo );
    m_fullscreen = foo;
    m_changed = true;
}

void is::Window::clear() {
    m_window->clear();
}

void is::Window::setIcon( std::string dir ) {
    is::File::Read file( dir );
    unsigned char* data = new unsigned char[ file.size() ];
    file.read( data, file.size() );

    sf::Image image;
    image.loadFromMemory( data, file.size() );
    delete data;

    m_window->setIcon( image.getSize().x, image.getSize().y, image.getPixelsPtr() );
}
