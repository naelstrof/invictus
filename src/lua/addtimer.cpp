int luaAddTimer( lua_State* l ) {
    //luaL_checkfunction( l, 1 );
    luaL_checktype( l, 1, LUA_TFUNCTION );
    float timeDiff = luaL_checknumber( l, 2 );

    lua_pushvalue( l, 1 );
    int ref = luaL_ref( l, LUA_REGISTRYINDEX );

    states->getCurrentState()->addTimer( ref, timeDiff );

    return 0;
}
