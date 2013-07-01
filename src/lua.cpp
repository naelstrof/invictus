#include "lua.hpp"

is::Lua* lua = new is::Lua();

#include "lua/print.cpp"

is::Lua::Lua() {
    m_l = luaL_newstate();

    // Load up all the lua libs.
    luaL_openlibs( m_l );

    // Unfortunately we can't just include lua functions in this file, we have to register them too. We'll do that here.
    addFunction( "print", luaPrint );
}

is::Lua::~Lua() {
    lua_close( m_l );
}

int is::Lua::init() {
    doFolder( "data/config" );
    // Lua should never fail, since it handles its own errors.
    return 0;
}

int is::Lua::doFile( sf::String dir ) {
    std::string actualdir;
    sf::Utf<32>::toUtf8( dir.begin(), dir.end(), back_inserter( actualdir ) );
    is::File::Read file( dir );
    if ( !file.good() ) {
        os->printf( "ERR Couldn't parse % as a lua file!\n", actualdir );
        return 1;
    }

    char* filedata = new char[ file.size() + 1 ]; // +1 for a terminating null character.
    file.read( filedata, file.size() );
    filedata[ file.size() ] = '\0'; // Ensure the 'string' is terminated just in case we're parsing some randomass file.

    // Finally parse the string and check for errors.
    int err = luaL_isdostring( m_l, filedata, actualdir.c_str() );
    delete[] filedata;
    if ( err ) {
        std::string error = lua_tostring( m_l, -1 );
        os->printf( "ERR %\n", error );
        return 1;
    }
    return 0;
}

int is::Lua::doFolder( sf::String dir ) {
    std::vector<std::string> files = filesystem->getFiles( dir );
    int results = 0;
    for ( unsigned int i=0;i<files.size();i++ ) {
        if ( files[i].find(".lua") == files[i].size()-4 ) {
            results += doFile( files[i] );
        }
    }
    return results;
}

void is::Lua::addFunction( sf::String name, lua_CFunction func ) {
    std::string actualname;
    sf::Utf<32>::toUtf8( name.begin(), name.end(), back_inserter( actualname ) );
    lua_register( m_l, actualname.c_str(), func );
}

int is::luaL_loadstring( lua_State* l, const char* s, const char* dir ) {
    return luaL_loadbuffer( l, s, strlen(s), dir );
}

float is::Lua::getFloat( sf::String name ) {
    std::string actualname;
    sf::Utf<32>::toUtf8( name.begin(), name.end(), back_inserter( actualname ) );

    lua_getglobal( m_l, "_G" );
    lua_getfield( m_l, -1, actualname.c_str() );
    if ( !lua_isnumber( m_l, -1 ) ) {
        os->printf( "WRN Lua tried to use % as a number. (It's not a number or doesn't exist!)\n", actualname );
        lua_pop( m_l, 2 );
        return 0;
    }
    float num = lua_tonumber( m_l, -1 );
    lua_pop( m_l, 2 );
    return num;
}

std::string is::Lua::getString( sf::String name ) {
    std::string actualname;
    sf::Utf<32>::toUtf8( name.begin(), name.end(), back_inserter( actualname ) );

    lua_getglobal( m_l, "_G" );
    lua_getfield( m_l, -1, actualname.c_str() );
    if ( !lua_isstring( m_l, -1 ) ) {
        os->printf( "WRN Lua tried to use % as a string. (It's not a string or doesn't exist!)\n", actualname );
        lua_pop( m_l, 2 );
        return "NULL";
    }
    std::string text = lua_tostring( m_l, -1 );
    lua_pop( m_l, 2 );
    return text;
}

bool is::Lua::getBool( sf::String name ) {
    std::string actualname;
    sf::Utf<32>::toUtf8( name.begin(), name.end(), back_inserter( actualname ) );

    lua_getglobal( m_l, "_G" );
    lua_getfield( m_l, -1, actualname.c_str() );
    if ( !lua_isboolean( m_l, -1 ) ) {
        os->printf( "WRN Lua tried to use % as a string. (It's not a string or doesn't exist!)\n", actualname );
        lua_pop( m_l, 2 );
        return false;
    }
    bool boolean = (bool)lua_toboolean( m_l, -1 );
    lua_pop( m_l, 2 );
    return boolean;
}
