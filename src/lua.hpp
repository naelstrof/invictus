#ifndef IS_LUA_H_
#define IS_LUA_H_

#include "os.hpp"
#include "filesystem.hpp"

#include <lua.hpp>

// Here we define our own lua dostring function that properly registers file location for error messages.
#define luaL_isdostring( l, s, n ) \
        (is::luaL_loadstring( l, s, n ) || lua_pcall( l, 0, LUA_MULTRET, 0 ))

namespace is {

class Lua {
private:
    lua_State*      m_l;
public:
    Lua();
    ~Lua();
    int             init();
    void            tick();
    int             doFile( sf::String dir );
    int             doFolder( sf::String dir );
    void            addFunction( sf::String name, lua_CFunction func );
    float           getFloat( sf::String name );
    std::string     getString( sf::String name );
    bool            getBool( sf::String name );
};

int luaL_loadstring( lua_State* l, const char* s, const char* dir );

};

extern is::Lua* lua;

#endif // IS_LUA_H_
