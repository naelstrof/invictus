#include "../nodes/text.hpp"

is::Text* lua_totext( lua_State* l, int index ) {
    is::Text** text = (is::Text**)luaL_checkudata( l, index, "TextBase" );
    return *text;
}

is::Text* lua_checktext( lua_State* l, int narg )
{
    is::Text* foo = lua_totext( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL Text!" );
    }
    return foo;
}

void lua_pushtext( lua_State* l, is::Text* text )
{
    is::Text** pointer = (is::Text**)lua_newuserdata( l, sizeof(is::Text*) );
    *pointer = text;
    luaL_getmetatable( l, "TextBase" );
    lua_setmetatable( l,-2 );
}

int luaText__gc( lua_State* l ) {
    is::Text* text = lua_checktext( l, 1 );
    gui->remove( text );
    return 0;
}

int luaTextRemove( lua_State* l ) {
    is::Text* text = lua_checktext( l, 1 );
    gui->remove( text );
    return 0;
}

int luaText__index( lua_State* l ) {
    is::Text* text = lua_totext(l,1);
    if ( text == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Text!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "pos" ) {
        lua_pushvector( l, text->getPos() );
        return 1;
    } else if ( field == "ang" ) {
        lua_pushvector( l, text->getAng() );
        return 1;
    } else if ( field == "color" ) {
        lua_pushcolor( l, text->getColor() );
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

int luaText__newindex(lua_State* l) {
    is::Text* text = lua_totext(l,1);
    if ( text == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Text!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "pos" ) {
        text->setPos( *lua_checkvector( l, 3 ) );
    } else if ( field ==  "ang" ) {
        text->setAng( *lua_checkvector( l, 3 ) );
    } else if ( field ==  "color" ) {
        text->setColor( *lua_checkcolor( l, 3 ) );
    } else {
        if ( text->m_luaReference == LUA_NOREF ) {
            lua_newtable( l );
            text->m_luaReference = luaL_ref( l, LUA_REGISTRYINDEX );
        }
        lua_rawgeti( l, LUA_REGISTRYINDEX, text->m_luaReference );
        lua_pushvalue( l, 3 );
        lua_setfield( l, -2, luaL_checkstring( l, 2 ) );
    }

    return 0;
}

int luaCreateText( lua_State* l ) {
    is::Text* text = new is::Text( luaL_checkstring( l, 1 ) );
    gui->addNode( text );
    lua_pushtext( l, text );
    return 1;
}

int luaRegisterTexts( lua_State* l ) {
    // Create a new table called TextBase
    luaL_newmetatable( l, "TextBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg textLib[] {
        { "remove", luaTextRemove },
        { "__index", luaText__index },
        { "__newindex", luaText__newindex },
        { "__gc", luaText__gc },
        { NULL, NULL }
    };
    luaL_register( l, NULL, textLib );
    // Set its type to Text
    lua_pushstring( l, "Text" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);

    // Create a function that puts a new is::Text on the stack.
    lua_register( l, "Text", luaCreateText );
    return 0;
}
