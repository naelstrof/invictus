is::Sound* lua_tosound( lua_State* l, int index ) {
    is::Sound** sound = (is::Sound**)luaL_checkudata( l, index, "SoundBase" );
    return *sound;
}

is::Sound* lua_checksound( lua_State* l, int narg )
{
    is::Sound* foo = lua_tosound( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL Sound!" );
    }
    return foo;
}

void lua_pushsound( lua_State* l, is::Sound* sound )
{
    is::Sound** pointer = (is::Sound**)lua_newuserdata( l, sizeof(is::Sound*) );
    *pointer = sound;
    luaL_getmetatable( l, "SoundBase" );
    lua_setmetatable( l,-2 );
}

int luaSound__gc( lua_State* l ) {
    is::Sound* sound = lua_checksound( l, 1 );
    delete sound;
    is::Sound** realsound = (is::Sound**)luaL_checkudata( l, 1, "SoundBase" );
    (*realsound) = NULL;
    return 0;
}

int luaSoundRemove( lua_State* l ) {
    is::Sound* sound = lua_checksound( l, 1 );
    delete sound;
    is::Sound** realsound = (is::Sound**)luaL_checkudata( l, 1, "SoundBase" );
    (*realsound) = NULL;
    return 0;
}

int luaSoundPlay( lua_State* l ) {
    is::Sound* sound = lua_checksound( l, 1 );
    sound->play();
    return 0;
}

int luaSoundPause( lua_State* l ) {
    is::Sound* sound = lua_checksound( l, 1 );
    sound->pause();
    return 0;
}

int luaSoundStop( lua_State* l ) {
    is::Sound* sound = lua_checksound( l, 1 );
    sound->stop();
    return 0;
}

int luaSound__index( lua_State* l ) {
    is::Sound* sound = lua_tosound(l,1);
    if ( sound == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Sound!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "pos" ) {
        lua_pushvector( l, sound->getPos() );
        return 1;
    } else if ( field == "x" ) {
        lua_pushnumber( l, sound->getPos().x );
        return 1;
    } else if ( field == "y" ) {
        lua_pushnumber( l, sound->getPos().y );
        return 1;
    } else if ( field == "z" ) {
        lua_pushnumber( l, sound->getPos().z );
        return 1;
    } else if ( field == "pitch" ) {
        lua_pushnumber( l, sound->getPitch() );
        return 1;
    }
    lua_getmetatable( l, 1 );
    lua_pushvalue( l, 2 );
    lua_gettable( l, -2 );
    if ( lua_isnil( l, -1 ) ) {
        lua_pop( l, 1 );
        lua_rawgeti( l, LUA_REGISTRYINDEX, sound->m_luaReference );
        lua_pushvalue( l, 2 );
        lua_gettable( l, -2 );
    }
    return 1;
}

int luaSound__newindex(lua_State* l) {
    is::Sound* sound = lua_tosound(l,1);
    if ( sound == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Sound!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "pos" ) {
        sound->setPos( *lua_checkvector( l, 3 ) );
    } else if ( field == "x" ) {
        glm::vec3 pos = sound->getPos();
        pos.x = luaL_checknumber( l, 3 );
        sound->setPos( pos );
    } else if ( field == "y" ) {
        glm::vec3 pos = sound->getPos();
        pos.y = luaL_checknumber( l, 3 );
        sound->setPos( pos );
    } else if ( field == "z" ) {
        glm::vec3 pos = sound->getPos();
        pos.z = luaL_checknumber( l, 3 );
        sound->setPos( pos );
    } else if ( field == "pitch" ) {
        sound->setPitch( luaL_checknumber( l, 3 ) );
    } else {
        if ( sound->m_luaReference == LUA_NOREF ) {
            lua_newtable( l );
            sound->m_luaReference = luaL_ref( l, LUA_REGISTRYINDEX );
        }
        lua_rawgeti( l, LUA_REGISTRYINDEX, sound->m_luaReference );
        lua_pushvalue( l, 3 );
        lua_setfield( l, -2, luaL_checkstring( l, 2 ) );
    }

    return 0;
}

int luaCreateSound( lua_State* l ) {
    is::Sound* sound = new is::Sound( luaL_checkstring( l, 1 ) );
    lua_pushsound( l, sound );
    return 1;
}

int luaRegisterSounds( lua_State* l ) {
    // Create a new table called SoundBase
    luaL_newmetatable( l, "SoundBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg soundLib[] {
        { "remove", luaSoundRemove },
        { "play", luaSoundPlay },
        { "pause", luaSoundPause },
        { "stop", luaSoundStop },
        { "__index", luaSound__index },
        { "__newindex", luaSound__newindex },
        { "__gc", luaSound__gc },
        { NULL, NULL }
    };
    luaL_register( l, NULL, soundLib );
    // Set its type to Sound
    lua_pushstring( l, "Sound" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);

    lua_register( l, "Sound", luaCreateSound );
    return 0;
}
