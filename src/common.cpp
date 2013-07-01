#include "common.hpp"

is::Common::Common() {
    m_running = true;
}

is::Common::~Common() {
    window->close();
}

is::Common* common = new is::Common();

int is::Common::init( int argc, char** argv ) {
    boost::nowide::args( argc, argv );

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
    return 0;
}

void is::Common::tick() {
    if ( keyboard->isDown( is::Key::Escape ) ) {
        m_running = false;
        return;
    }
    if ( !window->isOpen() ) {
        m_running = false;
        return;
    }
    window->tick(0);
    window->swapBuffers();
}
