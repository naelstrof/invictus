// os.hpp: Handles OS SPECIFIC calls only, like spawning threads, locking memory, printing to console, grabbing system information, etc.

#ifndef IS_OS_H_
#define IS_OS_H_

#include <stdexcept>
#include <iostream>
#include <boost/nowide/args.hpp>
#include <boost/nowide/iostream.hpp>

namespace is {

class OS {
public:
    int             init( int argc, char** argv );
    int             printf( const char* s );
template<typename T, typename... Args>
    int             printf( const char* s, T value, Args... args );
    void            flush();
    void            help();
};

// Define template functions, since they can't be defined in cpp files.
#include "os_templates.cpp"

};

extern is::OS* os;

#endif // IS_OS_H_
