int luaAddFont( lua_State* l ) {
    std::string fontname = luaL_checkstring( l, 1 );
    std::string dir = luaL_checkstring( l, 2 );

    fonts->addFont( fontname, dir );

    return 0;
}
