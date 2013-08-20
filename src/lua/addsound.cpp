int luaAddSound( lua_State* l ) {
    std::string soundname = luaL_checkstring( l, 1 );
    std::string filedir = luaL_checkstring( l, 2 );

    audio->addSoundBuffer( soundname, filedir );

    return 0;
}
