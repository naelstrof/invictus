int luaGetWindowWidth( lua_State* l ) {
    lua_pushnumber( l, window->getWidth() );
    return 1;
}

int luaGetWindowHeight( lua_State* l ) {
    lua_pushnumber( l, window->getHeight() );
    return 1;
}

int luaGetWindowModes( lua_State* l ) {
    lua_getglobal( l, "WindowModes" );
    if ( lua_isnil( l, -1 ) ) {
        lua_newtable( l );
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
        unsigned int highestBits = 0;
        for ( unsigned int i=0;i<modes.size();i++ ) {
            if ( modes[i].bitsPerPixel > highestBits ) {
                highestBits = modes[i].bitsPerPixel;
            }
        }
        for ( unsigned int i=0; i<modes.size();i++ ) {
            if ( modes[i].bitsPerPixel != highestBits ) {
                continue;
            }
            lua_pushnumber( l, i+1 );
            std::stringstream foo;
            foo << modes[i].width;
            foo << "x";
            foo << modes[i].height;
            lua_pushstring( l, foo.str().c_str() );
            lua_settable( l, -3 );
        }
        lua_setglobal( l, "WindowModes" );
        lua_getglobal( l, "WindowModes" );
    }
    return 1;
}
