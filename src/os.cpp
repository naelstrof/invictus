#include "os.hpp"

is::OS* os = new is::OS();

int is::OS::init( int argc, char** argv ) {
    boost::nowide::args( argc, argv );
    printf( "INF Invictus version: %.%.%\n", 1, 0, 0 );
    return 0;
}

void is::OS::exit( std::string message ) {
    throw std::runtime_error( message );
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

//Implemented in os_templates.cpp
//int is::OS::printf( const char* s, T value, Args... args ){}
