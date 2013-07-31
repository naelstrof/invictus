int luaAddShader( lua_State* l ) {
    std::string shadername = luaL_checkstring( l, 1 );
    std::string vertdir = luaL_checkstring( l, 2 );
    std::string fragdir = luaL_checkstring( l, 3 );
    std::string shadertype = luaL_checkstring( l, 4 );

    shaders->addShader( shadername, vertdir, fragdir, shadertype );

    return 0;
}
