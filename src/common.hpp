// common.hpp: System that handles starting up and shutting down all other systems.
#ifndef IS_COMMON_H_
#define IS_COMMON_H_

#include "os.hpp"
#include "window.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "filesystem.hpp"
#include "lua.hpp"
#include "scene.hpp"
#include "render.hpp"
#include "shader.hpp"
#include "textureloader.hpp"
#include "statemachine.hpp"
#include "model.hpp"
#include "audio.hpp"
#include "font.hpp"
#include "glyph.hpp"
#include "nodes/icon.hpp"

namespace is {

class Common {
private:
    sf::Clock m_dt;
public:
    Common();
    ~Common();
    int             init( int argc, char** argv );
    void            tick();
    bool            m_running;
    float           getGameTime();
    float           getDeltaTime();
};

};

extern is::Common* common;

#endif // IS_COMMON_H_
