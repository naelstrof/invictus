#include "common.hpp"

is::Common::Common() {
    m_running = false;
}

is::Common::~Common() {
    window->close();
}

is::Common* common = new is::Common();

int is::Common::init( int argc, char** argv ) {
    boost::nowide::args( argc, argv );

    // Parse game options.
    int width = 0;
    int height = 0;
    bool fullscreen = false;
    bool noborder = false;
    for ( int i=0; i<argc; i++ ) {
        std::string arg = argv[i];
        if ( arg == "-w" || arg == "--width" ) {
            width = atoi( argv[i+1] );
            i++;
            continue;
        }
        if ( arg == "-h" || arg == "--height" ) {
            height = atoi( argv[i+1] );
            i++;
            continue;
        }
        if ( arg == "--fullscreen" ) {
            fullscreen = true;
            continue;
        }
        if ( arg == "--windowed" ) {
            fullscreen = false;
            continue;
        }
        if ( arg == "--noborder" ) {
            noborder = true;
            continue;
        }
    }

    // Finally initialize the game.
    int err = filesystem->init( argv[0] );
    if ( err ) {
        os->printf( "ERR Failed initialize the filesystem, shutting down...\n" );
        return err;
    }

    err = lua->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize lua, shutting down...\n" );
        return err;
    }

    err = window->init();
    if ( err ) {
        os->printf( "ERR Failed to open window, shutting down...\n" );
        return err;
    }
    if ( fullscreen ) {
        window->setFullscreen( true );
    } else {
        window->setFullscreen( false );
    }
    if ( noborder ) {
        window->setNoBorder( true );
    }
    if ( width || height ) {
        window->setSize( width, height );
    }

    err = glewInit();
    if ( err != GLEW_OK ) {
        os->printf( "ERR %\n", glewGetErrorString( err ) );
        os->printf( "ERR Failed to initialize glew, shutting down...\n" );
        return err;
    }

    err = shaders->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize shaders, shutting down...\n" );
        return err;
    }

    err = fonts->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize fonts, shutting down...\n" );
        return err;
    }

    err = models->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize models, shutting down...\n" );
        return err;
    }

    err = render->init();
    if ( err ) {
        os->printf( "ERR Failed to initialize render engine, shutting down...\n" );
        return err;
    }

    is::Text* text = new is::Text( "abcdefghijklmnopqrstuvwxyz!" );
    gui->addNode( text );

    text->setPos(256,256-32,-500);

    m_running = true;
    m_time.restart();
    return 0;
}

void is::Common::tick() {
    if ( keyboard->isDown( is::Key::Escape ) || keyboard->isDown( is::Key::Q ) ) {
        m_running = false;
        return;
    }
    if ( !window->isOpen() ) {
        m_running = false;
        return;
    }
    sf::Time dt = m_time.restart();
    window->tick();
    world->tick( dt.asSeconds() );
    gui->tick( dt.asSeconds() );
    render->tick();
    render->draw();
}
