int luaGetWindowWidth( lua_State* l ) {
    lua_pushnumber( l, window->getWidth() );
    return 1;
}

int luaGetWindowHeight( lua_State* l ) {
    lua_pushnumber( l, window->getHeight() );
    return 1;
}
