#include "lua.hpp"

is::Lua* lua = new is::Lua();

#include "lua/print.cpp"
#include "lua/addshader.cpp"
#include "lua/updatewindow.cpp"
#include "lua/addfont.cpp"
#include "lua/addmodel.cpp"
#include "lua/addsound.cpp"
#include "lua/addtexture.cpp"
#include "lua/states.cpp"
#include "lua/color.cpp"
#include "lua/vector.cpp"
#include "lua/text.cpp"
#include "lua/icon.cpp"
#include "lua/dropdown.cpp"
#include "lua/button.cpp"
#include "lua/checkbox.cpp"
#include "lua/node.cpp"
#include "lua/sound.cpp"
#include "lua/getwindow.cpp"
#include "lua/setstate.cpp"
#include "lua/addtimer.cpp"

is::Lua::Lua() {
    m_l = luaL_newstate();

    // First load the panic manager
    lua_atpanic( m_l, luaOnPanic );

    // Load up all the lua libs.
    luaL_openlibs( m_l );

    // Unfortunately we can't just include lua functions in this file, we have to register them too. We'll do that here.
    addFunction( "print", luaPrint );
    addFunction( "addShader", luaAddShader );
    addFunction( "addFont", luaAddFont );
    addFunction( "addModel", luaAddModel );
    addFunction( "addSound", luaAddSound );
    addFunction( "addTimer", luaAddTimer );
    luaRegisterAnimations( m_l );
    addFunction( "addTexture", luaAddTexture );
    luaRegisterStates( m_l );
    luaRegisterVectors( m_l );
    luaRegisterColors( m_l );
    luaRegisterNodes( m_l );
    luaRegisterIcons( m_l );
    luaRegisterTexts( m_l );
    luaRegisterSounds( m_l );
    luaRegisterButtons( m_l );
    luaRegisterCheckboxs( m_l );
    luaRegisterDropdowns( m_l );

    addFunction( "getWindowWidth", luaGetWindowWidth );
    addFunction( "getWindowHeight", luaGetWindowHeight );
    addFunction( "getWindowModes", luaGetWindowModes );
    addFunction( "setState", luaSetState );
    addFunction( "updateWindow", luaUpdateWindow );
}

is::Lua::~Lua() {
    lua_close( m_l );
}

int is::Lua::init() {
    doFolder( "data/config" );
    luaLoadStates( m_l );
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
        // We got to change the error message to reflect that we're actually getting an error in a file, rather than a string.
        std::size_t place = error.find( "[string" );
        while ( place != std::string::npos ) {
            error.replace( place, 7, "[file" );
            place = error.find( "[string" );
        }
        os->printf( "ERR %\n", error );
        return 1;
    }
    return 0;
}

int is::Lua::doFolder( std::string dir ) {
    std::vector<std::string> files = filesystem->getFiles( dir );
    if ( files.size() <= 0 ) {
        os->printf( "WRN Couldn't find anything in %, does it exist?\n", dir );
        return 1;
    }
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

int is::Lua::call( lua_State* l, int nargs, int nresults ) {
    if ( lua_pcall( l, nargs, nresults, 0 ) ) {
        std::string error = lua_tostring( l, -1 );
        std::size_t place = error.find( "[string" );
        while ( place != std::string::npos ) {
            error.replace( place, 7, "[file" );
            place = error.find( "[string" );
        }
        os->printf( "ERR Lua: %\n", error );

        traceback( l );
        if ( error != lua_tostring( l, -1 ) ) {
            error = lua_tostring( l, -1 );
            place = error.find( "[string" );
            while ( place != std::string::npos ) {
                error.replace( place, 7, "[file" );
                place = error.find( "[string" );
            }
            os->printf( "ERR Lua: %\n", error );
        }
        return 1;
    }
    return 0;
}

int is::Lua::traceback( lua_State* l ) {
    if ( !lua_isstring( l, 1 ) ) {
        return 1;
    }
    lua_getglobal( l, "debug" );
    if ( !lua_istable( l, -1 ) ) {
        lua_pop( l, 1 );
        return 1;
    }
    lua_getfield( l, -1, "traceback" );
    if ( !lua_isfunction( l, -1 ) ) {
        lua_pop( l, 2 );
        return 1;
    }
    lua_pushvalue( l, 1 );
    lua_pushinteger( l, 2 );
    lua_call( l, 2, 1 );
    return 1;
}

int is::luaOnPanic( lua_State* l ) {
    std::string error = lua_tostring( l, -1 );
    os->printf( "ERR %\n", error );
    lua->traceback( l );
    if ( error != lua_tostring( l, -1 ) ) {
        error = lua_tostring( l, -1 );
        os->printf( "ERR %\n", error );
    }
    return 0;
}

void is::Lua::copy( lua_State* l, int index ) {
    lua_newtable( l );
    lua_pushnil( l );
    while ( lua_next( l, index ) != 0 ) {
        lua_pushvalue( l, -2 );
        lua_insert( l, -2 );
        lua_settable( l, -4 );
    }
}

void is::Lua::copyMeta( lua_State* l, int index, std::string newname ) {
    luaL_newmetatable( l, newname.c_str() );
    lua_pushnil( l );
    while ( lua_next( l, index ) != 0 ) {
        lua_pushvalue( l, -2 );
        lua_insert( l, -2 );
        lua_settable( l, -4 );
    }
}
