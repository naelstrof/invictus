is::Animation* lua_toAnimation( lua_State* l, int index ) {
    is::Animation** animation = (is::Animation**)luaL_checkudata( l, index, "AnimationBase" );
    return *animation;
}

is::Animation* lua_checkAnimation( lua_State* l, int narg )
{
    is::Animation* foo = lua_toAnimation( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL Animation!" );
    }
    return foo;
}

void lua_pushAnimation( lua_State* l, is::Animation* animation )
{
    is::Animation** pointer = (is::Animation**)lua_newuserdata( l, sizeof(is::Animation*) );
    *pointer = animation;
    luaL_getmetatable( l, "AnimationBase" );
    lua_setmetatable( l,-2 );
}

int luaAnimation__gc( lua_State* l ) {
    is::Animation* animation = lua_checkAnimation( l, 1 );
    delete animation;
    return 0;
}

int luaAnimation__newindex(lua_State* l) {
    is::Animation* animation = lua_toAnimation(l,1);
    if ( animation == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Animation!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    const char* field = luaL_checkstring(l,2);
    if ( !strcmp( field, "fps" ) ) {
        animation->m_fps = luaL_checknumber( l, 3 );
    } else if ( !strcmp( field, "name" ) ) {
        animation->m_name = luaL_checkstring( l, 3 );
    } else if ( !strcmp( field, "loop" ) ) {
        animation->m_loop = lua_toboolean( l, 3 );
    } else {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to create new index on an Animation! (Animations can't hold random data! Use fps, name, or loop instead.)", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }

    return 0;
}

int luaCreateAnimation( lua_State* l ) {
    unsigned int argcount = lua_gettop( l );
    if ( argcount <= 1 ) {
        return 0;
    }
    is::Animation* animation = new is::Animation( luaL_checkstring( l, 1 ) );
    for (unsigned int i=2;i<1+argcount;i++) {
        const char* field = luaL_checkstring(l,i);
        animation->addFrame( field );
    }
    lua_pushAnimation( l, animation );
    return 1;
}

int luaRegisterAnimations( lua_State* l ) {
    // Create a new table called AnimationBase
    luaL_newmetatable( l, "AnimationBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg animationLib[] {
        { "__newindex", luaAnimation__newindex },
        { "__gc", luaAnimation__gc },
        { NULL, NULL }
    };
    luaL_register( l, NULL, animationLib );
    // Set its type to Animation
    lua_pushstring( l, "Animation" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);

    // Create a function that puts a new is::Animation on the stack.
    lua_register( l, "Animation", luaCreateAnimation );
    return 0;
}

int luaAddTexture( lua_State* l ) {
    unsigned int argcount = lua_gettop( l );
    is::Texture* texture = new is::Texture( luaL_checkstring( l, 1 ) );
    for ( unsigned int i=2; i<1+argcount; i++ ) {
        texture->addAnimation( *lua_checkAnimation( l, i ) );
    }
    textures->addTexture( texture );
    return 0;
}
