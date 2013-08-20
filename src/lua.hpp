#ifndef IS_LUA_H_
#define IS_LUA_H_

// Required to use luaL_register, not sure of any alternatives.
#define LUA_COMPAT_MODULE
#include <lua.hpp>

#include "os.hpp"
#include "textureloader.hpp"
#include "filesystem.hpp"
#include "shaderloader.hpp"
#include "font.hpp"
#include "model.hpp"
#include "audio.hpp"
#include "sound.hpp"
#include "statemachine.hpp"

// Here we define our own lua dostring function that properly registers file location for error messages.
#define luaL_isdostring( l, s, n ) \
        (is::luaL_loadstring( l, s, n ) || lua_pcall( l, 0, LUA_MULTRET, 0 ))

namespace is {

class Lua {
public:
    lua_State*      m_l;
    Lua();
    ~Lua();
    int             init();
    void            tick();
    int             call( lua_State* l, int nargs, int nresults, std::string errormessage = "Lua's C API had an error" );
    int             doFile( std::string dir );
    int             doFolder( std::string dir );
    void            addFunction( std::string name, lua_CFunction func );
    float           getFloat( std::string name );
    std::string     getString( std::string name );
    bool            getBool( std::string name );
    void            setFloat( std::string name, float foo );
    void            setString( std::string name, std::string foo );
    void            setBool( std::string name, bool foo );
    void            copy( lua_State* l, int index );
    void            copyMeta( lua_State* l, int index, std::string newname );
};

int luaL_loadstring( lua_State* l, const char* s, const char* dir );
int luaOnPanic( lua_State* l );

};

extern is::Lua* lua;

#endif // IS_LUA_H_
