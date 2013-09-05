is::Scene* lua_toscene( lua_State* l, int index ) {
    is::Scene** scene = (is::Scene**)luaL_checkudata( l, index, "SceneBase" );
    return *scene;
}

is::Scene* lua_checkscene( lua_State* l, int narg )
{
    is::Scene* foo = lua_toscene( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL Scene!" );
    }
    return foo;
}

void lua_pushscene( lua_State* l, is::Scene* scene )
{
    is::Scene** pointer = (is::Scene**)lua_newuserdata( l, sizeof(is::Scene*) );
    *pointer = scene;
    luaL_getmetatable( l, "SceneBase" );
    lua_setmetatable( l,-2 );
}

int luaSceneAddNode( lua_State* l ) {
    is::Scene* scene = lua_toscene( l, 1 );
    scene->addNode( lua_checknode( l, 2 ) );
    return 0;
}

int luaGetScene( lua_State* l ) {
    std::string name = lua_tostring( l, 1 );
    if ( name == "world" ) {
        lua_rawgeti( l, LUA_REGISTRYINDEX, world->m_luaReference );
        return 1;
    } else if ( name == "gui" ) {
        lua_rawgeti( l, LUA_REGISTRYINDEX, gui->m_luaReference );
        return 1;
    }
    lua_pushnil( l );
    return 1;
}

int luaScene__index( lua_State* l ) {
    is::Scene* scene = lua_toscene(l,1);
    if ( scene == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Scene!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    lua_getmetatable( l, 1 );
    lua_pushvalue( l, 2 );
    lua_gettable( l, -2 );
    if ( lua_isnil( l, -1 ) ) {
        lua_pop( l, 1 );
        lua_rawgeti( l, LUA_REGISTRYINDEX, scene->m_luaReference );
        lua_pushvalue( l, 2 );
        lua_gettable( l, -2 );
    }
    return 1;
}

int luaScene__newindex(lua_State* l) {
    is::Scene* scene = lua_toscene(l,1);
    if ( scene == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Scene!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    if ( scene->m_luaReference == LUA_NOREF ) {
        lua_newtable( l );
        scene->m_luaReference = luaL_ref( l, LUA_REGISTRYINDEX );
    }
    lua_rawgeti( l, LUA_REGISTRYINDEX, scene->m_luaReference );
    lua_pushvalue( l, 3 );
    lua_setfield( l, -2, luaL_checkstring( l, 2 ) );

    return 0;
}

int luaRegisterScenes( lua_State* l ) {
    // Create a new table called SceneBase
    luaL_newmetatable( l, "SceneBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg sceneLib[] {
        { "__index", luaScene__index },
        { "__newindex", luaScene__newindex },
        { "addNode", luaSceneAddNode },
        { NULL, NULL }
    };
    luaL_register( l, NULL, sceneLib );
    // Set its type to Scene
    lua_pushstring( l, "Scene" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);

    lua_register( l, "getScene", luaGetScene );

    return 0;
}
