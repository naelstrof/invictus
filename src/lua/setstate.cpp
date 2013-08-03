int luaSetState( lua_State* l ) {
    states->setState( luaL_checkstring( l, 1 ) );
    return 0;
}
