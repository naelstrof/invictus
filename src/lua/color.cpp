glm::vec4* lua_tocolor( lua_State* l, int index ) {
    glm::vec4* color = (glm::vec4*)luaL_checkudata( l, index, "ColorBase" );
    return color;
}

glm::vec4* lua_checkcolor( lua_State* l, int narg )
{
    glm::vec4* foo = lua_tocolor( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL Color!" );
    }
    return foo;
}

void lua_pushcolor( lua_State* l, glm::vec4 color )
{
    glm::vec4* pointer = (glm::vec4*)lua_newuserdata( l, sizeof(glm::vec4) );
    *pointer = color;
    luaL_getmetatable( l, "ColorBase" );
    lua_setmetatable( l,-2 );
}

int luaColor__index( lua_State* l ) {
    glm::vec4* color = lua_tocolor(l,1);
    if ( color == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Color!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "r" ) {
        lua_pushnumber( l, color->x );
        return 1;
    } else if ( field == "g" ) {
        lua_pushnumber( l, color->y );
        return 1;
    } else if ( field == "b" ) {
        lua_pushnumber( l, color->z );
        return 1;
    } else if ( field == "a" ) {
        lua_pushnumber( l, color->w );
        return 1;
    }
    lua_Debug ar1;
    lua_getstack( l, 1, &ar1 );
    lua_getinfo( l, "fl", &ar1 );
    lua_Debug ar2;
    lua_getinfo( l, ">S", &ar2 );
    lua_pushfstring( l, "%s:%d: attempt to create index on a Color! (You can't do this.)", ar2.short_src, ar1.currentline );
    return lua_error( l );
}

int luaColor__newindex(lua_State* l) {
    glm::vec4* color = lua_tocolor(l,1);
    if ( color == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Color!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "r" ) {
        color->x = luaL_checknumber( l, 3 );
    } else if ( field ==  "g" ) {
        color->y = luaL_checknumber( l, 3 );
    } else if ( field ==  "b" ) {
        color->z = luaL_checknumber( l, 3 );
    } else if ( field ==  "a" ) {
        color->w = luaL_checknumber( l, 3 );
    }

    return 0;
}

int luaColor__add( lua_State* l ) {
    glm::vec4* foo = lua_checkcolor( l, 1 );
    glm::vec4* bar = lua_checkcolor( l, 2 );
    lua_pushcolor( l, *foo + *bar );
    return 1;
}

int luaColor__sub( lua_State* l ) {
    glm::vec4* foo = lua_checkcolor( l, 1 );
    glm::vec4* bar = lua_checkcolor( l, 2 );
    lua_pushcolor( l, *foo - *bar );
    return 1;
}

int luaColor__mul( lua_State* l ) {
    glm::vec4* foo = lua_checkcolor( l, 1 );
    if ( lua_type( l, 2 ) != LUA_TNUMBER ) {
        glm::vec4* bar = lua_checkcolor( l, 2 );
        lua_pushcolor( l, (*foo) * (*bar) );
        return 1;
    } else {
        lua_pushcolor( l, *foo * luaL_checknumber( l, 2 ) );
        return 1;
    }
    return 1;
}

int luaColor__div( lua_State* l ) {
    glm::vec4* foo = lua_checkcolor( l, 1 );
    if ( lua_type( l, 2 ) != LUA_TNUMBER ) {
        glm::vec4* bar = lua_checkcolor( l, 2 );
        lua_pushcolor( l, *foo / *bar );
        return 1;
    } else {
        lua_pushcolor( l, *foo / luaL_checknumber( l, 2 ) );
        return 1;
    }
    return 0;
}

int luaCreateColor( lua_State* l ) {
    unsigned int argcount = lua_gettop( l );
    glm::vec4 color;
    if ( argcount == 1 ) {
        color = glm::vec4( luaL_checknumber( l, 1 ) );
        lua_pushcolor( l, color );
        return 1;
    } else if ( argcount == 3 ) {
        color = glm::vec4( luaL_checknumber( l, 1 ), luaL_checknumber( l, 2 ), luaL_checknumber( l, 3 ), 1 );
    }
    color = glm::vec4( luaL_checknumber( l, 1 ), luaL_checknumber( l, 2 ), luaL_checknumber( l, 3 ), luaL_checknumber( l, 4 ) );
    lua_pushcolor( l, color );
    return 1;
}

int luaRegisterColors( lua_State* l ) {
    // Create a new table called ColorBase
    luaL_newmetatable( l, "ColorBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg colorLib[] {
        { "__add", luaColor__add },
        { "__sub", luaColor__sub },
        { "__mul", luaColor__mul },
        { "__div", luaColor__div },
        { "__index", luaColor__index },
        { "__newindex", luaColor__newindex },
        { NULL, NULL }
    };
    luaL_register( l, NULL, colorLib );
    // Set its type to Color
    lua_pushstring( l, "Color" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);

    // Create a function that puts a new glm::vec4 on the stack.
    lua_register( l, "Color", luaCreateColor );
    return 0;
}
