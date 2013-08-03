#include "../nodes/icon.hpp"

is::Icon* lua_toicon( lua_State* l, int index ) {
    is::Icon** icon = (is::Icon**)luaL_checkudata( l, index, "IconBase" );
    return *icon;
}

is::Icon* lua_checkicon( lua_State* l, int narg )
{
    is::Icon* foo = lua_toicon( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL Icon!" );
    }
    return foo;
}

void lua_pushicon( lua_State* l, is::Icon* icon )
{
    is::Icon** pointer = (is::Icon**)lua_newuserdata( l, sizeof(is::Icon*) );
    *pointer = icon;
    luaL_getmetatable( l, "IconBase" );
    lua_setmetatable( l,-2 );
}

int luaIcon__gc( lua_State* l ) {
    is::Icon* icon = lua_checkicon( l, 1 );
    gui->remove( icon );
    return 0;
}

int luaIconRemove( lua_State* l ) {
    is::Icon* icon = lua_checkicon( l, 1 );
    gui->remove( icon );
    return 0;
}

int luaIcon__index( lua_State* l ) {
    is::Icon* icon = lua_toicon(l,1);
    if ( icon == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Icon!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "pos" ) {
        lua_pushvector( l, icon->getPos() );
        return 1;
    } else if ( field == "ang" ) {
        lua_pushvector( l, icon->getAng() );
        return 1;
    } else if ( field == "color" ) {
        lua_pushcolor( l, icon->getColor() );
        return 1;
    }

    lua_getmetatable( l, 1 );
    lua_pushvalue( l, 2 );
    lua_gettable( l, -2 );
    if ( lua_isnil( l, -1 ) ) {
        lua_pop( l, 1 );
        lua_rawgeti( l, LUA_REGISTRYINDEX, icon->m_luaReference );
        lua_pushvalue( l, 2 );
        lua_gettable( l, -2 );
    }
    return 1;
}

int luaIcon__newindex(lua_State* l) {
    is::Icon* icon = lua_toicon(l,1);
    if ( icon == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Icon!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "pos" ) {
        icon->setPos( *lua_checkvector( l, 3 ) );
    } else if ( field ==  "ang" ) {
        icon->setAng( *lua_checkvector( l, 3 ) );
    } else if ( field ==  "color" ) {
        icon->setColor( *lua_checkcolor( l, 3 ) );
    } else {
        if ( icon->m_luaReference == LUA_NOREF ) {
            lua_newtable( l );
            icon->m_luaReference = luaL_ref( l, LUA_REGISTRYINDEX );
        }
        lua_rawgeti( l, LUA_REGISTRYINDEX, icon->m_luaReference );
        lua_pushvalue( l, 3 );
        lua_setfield( l, -2, luaL_checkstring( l, 2 ) );
    }

    return 0;
}

int luaCreateIcon( lua_State* l ) {
    is::Icon* icon = new is::Icon( luaL_checkstring( l, 1 ) );
    gui->addNode( icon );
    lua_pushicon( l, icon );
    return 1;
}

int luaRegisterIcons( lua_State* l ) {
    // Create a new table called IconBase
    luaL_newmetatable( l, "IconBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg iconLib[] {
        { "remove", luaIconRemove },
        { "__index", luaIcon__index },
        { "__newindex", luaIcon__newindex },
        { "__gc", luaIcon__gc },
        { NULL, NULL }
    };
    luaL_register( l, NULL, iconLib );
    // Set its type to Icon
    lua_pushstring( l, "Icon" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);

    // Create a function that puts a new is::Icon on the stack.
    lua_register( l, "Icon", luaCreateIcon );
    return 0;
}
