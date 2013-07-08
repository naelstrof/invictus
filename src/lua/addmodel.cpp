int luaAddModel( lua_State* l ) {
    std::string modelname = luaL_checkstring( l, 1 );
    std::string filedir = luaL_checkstring( l, 2 );

    models->addModel( modelname, filedir );

    return 0;
}
