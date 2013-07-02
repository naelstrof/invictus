#include "os.hpp"

is::OS* os = new is::OS();

int is::OS::init( int argc, char** argv ) {
    boost::nowide::args( argc, argv );
    printf( "Invictus version: %.%.%\n\n", 1, 0, 0 );
    for ( int i=0;i<argc;i++ ) {
        if ( !strcmp( argv[i], "--help" ) ) {
            help();
            exit(0);
        }
    }
    printf( "INF Launch with --help or type help if needed.\n" );
    return 0;
}

void is::OS::help() {
    printf( "Usage: invictus [options]\n\n" );
    printf( "Options:\n" );
    printf( "    --help                 Display this help message and exit.\n" );
    printf( "    -w or --width #        Change display width.\n" );
    printf( "    -h or --height #       Change display height.\n" );
    printf( "    --fullscreen           Run game in fullscreen.\n" );
    printf( "    --noborder             Run game without a border.\n" );
    printf( "    --windowed             Run game windowed.\n" );
}

// Wrapper for printf as Window's printf doesn't support UTF-8. Seriously Windows?
int is::OS::printf( const char* s ) {
    while ( *s ) {
        if ( *s == '%' ) {
            if ( *(s + 1) == '%' ) {
                s++;
            } else {
                throw std::runtime_error("invalid format string: missing arguments");
            }
        }
        //std::cout << *s++;

        // Use boost nowide's console for proper Utf-8 output!
        boost::nowide::cout << *s++;
        // Simulate printf's flushing on newline.
        if ( *(s - 1) == '\n' ) {
            boost::nowide::cout.flush();
        }
    }
    return 0;
}

void is::OS::flush() {
    boost::nowide::cout.flush();
}


//Implemented in os_templates.cpp
//int is::OS::printf( const char* s, T value, Args... args ){}
