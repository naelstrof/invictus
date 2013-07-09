int luaAddShader( lua_State* l ) {
    std::string shadername = luaL_checkstring( l, 1 );
    std::string vertdir = luaL_checkstring( l, 2 );
    std::string fragdir = luaL_checkstring( l, 3 );

    shaders->addShader( shadername, vertdir, fragdir );

    return 0;
}
