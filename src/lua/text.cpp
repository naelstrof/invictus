// Depends on ../lua/node.cpp

int luaTextSet( lua_State* l ) {
    // TODO: Check and make sure it's actually a text node before attempting to access the member :u.
    is::Text* textnode = (is::Text*)lua_checknode( l, 1 );
    std::string text = luaL_checkstring( l, 2 );
    textnode->setText( text );
    return 0;
}

int luaCreateText( lua_State* l ) {
    is::Text* text = new is::Text( luaL_checkstring( l, 1 ) );
    gui->addNode( text );
    lua_pushnode( l, text );
    return 1;
}

int luaText__newindex( lua_State* l ) {
    is::Text* text = (is::Text*)lua_tonode( l, 1 );
    if ( text == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Node!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring( l, 2 );
    if ( field == "size" ) {
        text->setSize( luaL_checknumber( l, 3 ) );
        return 0;
    } else if ( field == "text" ) {
        text->setText( luaL_checkstring( l, 3 ) );
        return 0;
    }
    return 0;
}

int luaText__index( lua_State* l ) {
    is::Text* text = (is::Text*)lua_tonode( l, 1 );
    if ( text == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Node!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring( l, 2 );
    if ( field == "size" ) {
        lua_pushnumber( l, text->m_size );
        return 1;
    } else if ( field == "text" ) {
        lua_pushstring( l, text->getText().c_str() );
        return 1;
    }
    lua_getmetatable( l, 1 );
    lua_pushvalue( l, 2 );
    lua_gettable( l, -2 );
    if ( lua_isnil( l, -1 ) ) {
        lua_pop( l, 1 );
        lua_rawgeti( l, LUA_REGISTRYINDEX, text->m_luaReference );
        lua_pushvalue( l, 2 );
        lua_gettable( l, -2 );
    }
    return 1;
}

int luaRegisterTexts( lua_State* l ) {
    luaL_getmetatable( l, "NodeBase" );
    lua->copyMeta( l, -1, "TextBase" );

    // Register its new index function and garbage collection function
    static const luaL_Reg textLib[] {
        { "__index", luaText__index },
        { "__newindex", luaText__newindex },
        { NULL, NULL }
    };
    luaL_register( l, NULL, textLib );
    // Clear the stack
    lua_pop(l,2);

    // Create a function that puts a new is::Text on the stack.
    lua_register( l, "Text", luaCreateText );
    return 0;
}
