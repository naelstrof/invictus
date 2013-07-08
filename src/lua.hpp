#ifndef IS_LUA_H_
#define IS_LUA_H_

#include "os.hpp"
#include "filesystem.hpp"
#include "shader.hpp"
#include "font.hpp"
#include "model.hpp"

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
    int             doFile( std::string dir );
    int             doFolder( std::string dir );
    void            addFunction( std::string name, lua_CFunction func );
    float           getFloat( std::string name );
    std::string     getString( std::string name );
    bool            getBool( std::string name );
    void            setFloat( std::string name, float foo );
    void            setString( std::string name, std::string foo );
    void            setBool( std::string name, bool foo );
};

int luaL_loadstring( lua_State* l, const char* s, const char* dir );

};

extern is::Lua* lua;

#endif // IS_LUA_H_
