#include "engine.hpp"

int main( int argc, char** argv ) {
    // Here we check if we just need to simply print a help message and exit.
    os->init( argc, argv );

    // Here we parse options for starting up the game, like --novid, -w, -h, etc. (And then actually initialize the game.
    common->init( argc, argv );

    while ( common->m_running ) {
        common->tick();
    }

    cleanUp();

    return 0;
}
