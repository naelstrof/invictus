#include "lua.hpp"

is::Lua* lua = new is::Lua();

#include "lua/print.cpp"
#include "lua/addshader.cpp"
#include "lua/addfont.cpp"
#include "lua/addmodel.cpp"

is::Lua::Lua() {
    m_l = luaL_newstate();

    // Load up all the lua libs.
    luaL_openlibs( m_l );

    // Unfortunately we can't just include lua functions in this file, we have to register them too. We'll do that here.
    addFunction( "print", luaPrint );
    addFunction( "addShader", luaAddShader );
    addFunction( "addFont", luaAddFont );
    addFunction( "addModel", luaAddModel );
}

is::Lua::~Lua() {
    lua_close( m_l );
}

int is::Lua::init() {
    doFolder( "data/config" );
    // Lua should never fail, since it handles its own errors.
    return 0;
}

int is::Lua::doFile( std::string dir ) {
    is::File::Read file( dir );
    if ( !file.good() ) {
        os->printf( "ERR Couldn't parse % as a lua file!\n", dir );
        return 1;
    }

    char* filedata = new char[ file.size() + 1 ]; // +1 for a terminating null character.
    file.read( filedata, file.size() );
    filedata[ file.size() ] = '\0'; // Ensure the 'string' is terminated just in case we're parsing some randomass file.

    // Finally parse the string and check for errors.
    int err = luaL_isdostring( m_l, filedata, dir.c_str() );
    delete[] filedata;
    if ( err ) {
        std::string error = lua_tostring( m_l, -1 );
        os->printf( "ERR %\n", error );
        return 1;
    }
    return 0;
}

int is::Lua::doFolder( std::string dir ) {
    std::vector<std::string> files = filesystem->getFiles( dir );
    int results = 0;
    for ( unsigned int i=0;i<files.size();i++ ) {
        if ( files[i].find(".lua") == files[i].size()-4 ) {
            results += doFile( files[i] );
        }
    }
    return results;
}

void is::Lua::addFunction( std::string name, lua_CFunction func ) {
    lua_register( m_l, name.c_str(), func );
}

int is::luaL_loadstring( lua_State* l, const char* s, const char* dir ) {
    return luaL_loadbuffer( l, s, strlen(s), dir );
}

float is::Lua::getFloat( std::string name ) {
    lua_getglobal( m_l, "_G" );
    lua_getfield( m_l, -1, name.c_str() );
    if ( !lua_isnumber( m_l, -1 ) ) {
        os->printf( "WRN Lua tried to use % as a number. (It's not a number or doesn't exist!)\n", name );
        lua_pop( m_l, 2 );
        return 0;
    }
    float num = lua_tonumber( m_l, -1 );
    lua_pop( m_l, 2 );
    return num;
}

std::string is::Lua::getString( std::string name ) {
    lua_getglobal( m_l, "_G" );
    lua_getfield( m_l, -1, name.c_str() );
    if ( !lua_isstring( m_l, -1 ) ) {
        os->printf( "WRN Lua tried to use % as a string. (It's not a string or doesn't exist!)\n", name );
        lua_pop( m_l, 2 );
        return "NULL";
    }
    std::string text = lua_tostring( m_l, -1 );
    lua_pop( m_l, 2 );
    return text;
}

bool is::Lua::getBool( std::string name ) {
    lua_getglobal( m_l, "_G" );
    lua_getfield( m_l, -1, name.c_str() );
    if ( !lua_isboolean( m_l, -1 ) ) {
        os->printf( "WRN Lua tried to use % as a string. (It's not a string or doesn't exist!)\n", name );
        lua_pop( m_l, 2 );
        return false;
    }
    bool boolean = (bool)lua_toboolean( m_l, -1 );
    lua_pop( m_l, 2 );
    return boolean;
}

void is::Lua::setString( std::string name, std::string foo ) {
    lua_getglobal( m_l, "_G" );
    lua_pushstring( m_l, foo.c_str() );
    lua_setfield( m_l, -2, name.c_str() );
    lua_pop( m_l, 1 );
}

void is::Lua::setBool( std::string name, bool foo ) {
    lua_getglobal( m_l, "_G" );
    lua_pushboolean( m_l, foo );
    lua_setfield( m_l, -2, name.c_str() );
    lua_pop( m_l, 1 );
}

void is::Lua::setFloat( std::string name, float foo ) {
    lua_getglobal( m_l, "_G" );
    lua_pushnumber( m_l, foo );
    lua_setfield( m_l, -2, name.c_str() );
    lua_pop( m_l, 1 );
}
