glm::vec3* lua_tovector( lua_State* l, int index ) {
    glm::vec3* vector = (glm::vec3*)luaL_checkudata( l, index, "VectorBase" );
    return vector;
}

glm::vec3* lua_checkvector( lua_State* l, int narg )
{
    glm::vec3* foo = lua_tovector( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL Vector!" );
    }
    return foo;
}

void lua_pushvector( lua_State* l, glm::vec3 vector )
{
    glm::vec3* pointer = (glm::vec3*)lua_newuserdata( l, sizeof(glm::vec3) );
    *pointer = vector;
    luaL_getmetatable( l, "VectorBase" );
    lua_setmetatable( l,-2 );
}

int luaVector__index( lua_State* l ) {
    glm::vec3* vector = lua_tovector(l,1);
    if ( vector == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Vector!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "x" ) {
        lua_pushnumber( l, vector->x );
        return 1;
    } else if ( field == "y" ) {
        lua_pushnumber( l, vector->y );
        return 1;
    } else if ( field == "z" ) {
        lua_pushnumber( l, vector->z );
        return 1;
    }
    lua_Debug ar1;
    lua_getstack( l, 1, &ar1 );
    lua_getinfo( l, "fl", &ar1 );
    lua_Debug ar2;
    lua_getinfo( l, ">S", &ar2 );
    lua_pushfstring( l, "%s:%d: attempt to create index on a Vector! (You can't do this.)", ar2.short_src, ar1.currentline );
    return lua_error( l );
}

int luaVector__newindex(lua_State* l) {
    glm::vec3* vector = lua_tovector(l,1);
    if ( vector == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Vector!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "x" ) {
        vector->x = luaL_checknumber( l, 3 );
    } else if ( field ==  "y" ) {
        vector->y = luaL_checknumber( l, 3 );
    } else if ( field ==  "z" ) {
        vector->z = luaL_checknumber( l, 3 );
    }

    return 0;
}

int luaVector__add( lua_State* l ) {
    glm::vec3* foo = lua_checkvector( l, 1 );
    glm::vec3* bar = lua_checkvector( l, 2 );
    lua_pushvector( l, *foo + *bar );
    return 1;
}

int luaVector__sub( lua_State* l ) {
    glm::vec3* foo = lua_checkvector( l, 1 );
    glm::vec3* bar = lua_checkvector( l, 2 );
    lua_pushvector( l, *foo - *bar );
    return 1;
}

int luaVector__mul( lua_State* l ) {
    glm::vec3* foo = lua_checkvector( l, 1 );
    if ( lua_type( l, 2 ) != LUA_TNUMBER ) {
        glm::vec3* bar = lua_checkvector( l, 2 );
        lua_pushvector( l, (*foo) * (*bar) );
        return 1;
    } else {
        lua_pushvector( l, (*foo) * float( luaL_checknumber( l, 2 ) ) );
        return 1;
    }
    return 1;
}

int luaVector__div( lua_State* l ) {
    glm::vec3* foo = lua_checkvector( l, 1 );
    if ( lua_type( l, 2 ) != LUA_TNUMBER ) {
        glm::vec3* bar = lua_checkvector( l, 2 );
        lua_pushvector( l, *foo / *bar );
        return 1;
    } else {
        lua_pushvector( l, (*foo) / float( luaL_checknumber( l, 2 ) ) );
        return 1;
    }
    return 0;
}

int luaCreateVector( lua_State* l ) {
    unsigned int argcount = lua_gettop( l );
    if ( argcount == 1 ) {
        glm::vec3 vector = glm::vec3( luaL_checknumber( l, 1 ) );
        lua_pushvector( l, vector );
        return 1;
    }
    glm::vec3 vector = glm::vec3( luaL_checknumber( l, 1 ), luaL_checknumber( l, 2 ), luaL_checknumber( l, 3 ) );
    lua_pushvector( l, vector );
    return 1;
}

int luaRegisterVectors( lua_State* l ) {
    // Create a new table called VectorBase
    luaL_newmetatable( l, "VectorBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg vectorLib[] {
        { "__add", luaVector__add },
        { "__sub", luaVector__sub },
        { "__mul", luaVector__mul },
        { "__div", luaVector__div },
        { "__index", luaVector__index },
        { "__newindex", luaVector__newindex },
        { NULL, NULL }
    };
    luaL_register( l, NULL, vectorLib );
    // Set its type to Vector
    lua_pushstring( l, "Vector" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);

    // Create a function that puts a new glm::vec3 on the stack.
    lua_register( l, "Vector", luaCreateVector );
    return 0;
}
