// common.hpp: System that handles starting up and shutting down all other systems.
#ifndef IS_COMMON_H_
#define IS_COMMON_H_

#include "os.hpp"
#include "window.hpp"
#include "keyboard.hpp"

namespace is {

class Common {
public:
    Common();
    ~Common();
    int             init( int argc, const char** argv );
    void            tick();
    bool            m_running;
};

};

extern is::Common* common;

#endif // IS_COMMON_H_
