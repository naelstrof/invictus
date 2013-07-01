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

    int err = window->init();
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
