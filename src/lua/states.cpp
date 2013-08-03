is::State* lua_tostate( lua_State* l, int index ) {
    is::State** state = (is::State**)luaL_checkudata( l, index, "State" );
    return *state;
}

is::State* lua_checkstate( lua_State* l, int narg ) {
    is::State* foo = lua_tostate( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL State!" );
    }
    return foo;
}

void lua_pushstate( lua_State* l, is::State* state ) {
    is::State** pointer = (is::State**)lua_newuserdata( l, sizeof(is::State*) );
    *pointer = state;
    luaL_getmetatable( l, "State" );
    lua_setmetatable( l,-2 );
}

int luaState__index( lua_State* l ) {
    is::State* state = lua_tostate(l,1);
    if ( state == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL State!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }

    if (!strcmp( luaL_checkstring( l, 2 ), "name" ) ) {
        lua_pushstring( l, state->m_name.c_str() );
        return 1;
    }
    lua_getmetatable( l, 1 );
    lua_pushvalue( l, 2 );
    lua_gettable( l, -2 );
    if ( lua_isnil( l, -1 ) ) {
        lua_pop( l, 1 );
        lua_rawgeti( l, LUA_REGISTRYINDEX, state->m_luaReference );
        lua_pushvalue( l, 2 );
        lua_gettable( l, -2 );
    }
    return 1;
}

int luaState__newindex( lua_State* l ) {
    is::State* state = lua_tostate(l,1);
    if ( state == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL State!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }

    if (!strcmp( luaL_checkstring( l, 2 ), "name" ) ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to change State name. You may not do that.", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }

    lua_rawgeti( l, LUA_REGISTRYINDEX, state->m_luaReference );
    lua_pushvalue( l, 3 );
    lua_setfield( l, -2, luaL_checkstring( l, 2 ) );

    return 0;
}

int luaRegisterStates( lua_State* l ) {
    // Create a new table called State
    luaL_newmetatable( l, "State" );
    // Register its new index function and garbage collection function
    static const luaL_Reg stateLib[] {
        { "__index", luaState__index },
        { "__newindex", luaState__newindex },
        { NULL, NULL }
    };
    luaL_register( l, NULL, stateLib );
    // Set its type to State
    lua_pushstring( l, "State" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);
    return 0;
}

int luaLoadStates( lua_State* l ) {
    // Get a list of the folders in data/states.
    std::vector<std::string> statelist = filesystem->getFiles( "data/states", false );

    int errorcount = 0;

    for ( unsigned int i=0; i<statelist.size(); i++ ) {
        if ( !filesystem->isDirectory( statelist.at( i ) ) ) {
            continue;
        }
        std::string folder = statelist.at( i );
        std::string initfile = folder + "/init.lua";
        std::string statename = folder.substr( folder.find_last_of( '/' ) + 1 );

        // Create the new "State" table
        lua_newtable( l );
        // Then make it our new global named "STATE"
        lua_setglobal( l, "STATE" );
        if ( lua->doFile( initfile ) ) {
            // Oh shit we failed, abort! Setting the "STATE" global to nil will cause the old one to get deconstructed.
            lua_pushnil( l );
            lua_setglobal( l, "STATE" );
            errorcount++;
            continue;
        }
        lua_getglobal( l, "STATE" );
        int reference = luaL_ref( l, LUA_REGISTRYINDEX );
        is::State* state = new is::State( statename, reference );
        // Then finally send it to the state machine
        states->addState( state );
    }
    return errorcount;
}
