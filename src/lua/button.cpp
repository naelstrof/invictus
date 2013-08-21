#include "../nodes/button.hpp"

// Depends on ./lua/node.cpp
void lua_pushnode( lua_State* l, is::Node* node );
is::Node* lua_tonode( lua_State* l, int index );
is::Node* lua_checknode( lua_State* l, int index );

int luaButton__index( lua_State* l ) {
    is::Button* button = (is::Button*)lua_tonode(l,1);
    std::string field = luaL_checkstring(l,2);
    if ( field == "onPress" ) {
        lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, button->m_luaPressFunction );
        return 1;
    } else if ( field == "onRelease" ) {
        lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, button->m_luaReleaseFunction );
        return 1;
    }
    return 0;
}

int luaButton__newindex( lua_State* l ) {
    is::Button* button = (is::Button*)lua_tonode(l,1);
    std::string field = luaL_checkstring(l,2);
    if ( field == "onPress" ) {
        luaL_checktype( l, 3, LUA_TFUNCTION );
        lua_pushvalue( l, 3 );
        button->m_luaPressFunction = luaL_ref( lua->m_l, LUA_REGISTRYINDEX );
    } else if ( field == "onRelease" ) {
        luaL_checktype( l, 3, LUA_TFUNCTION );
        lua_pushvalue( l, 3 );
        button->m_luaReleaseFunction = luaL_ref( lua->m_l, LUA_REGISTRYINDEX );
    }
    return 0;
}

int luaCreateButton( lua_State* l ) {
    int arg = lua_gettop( l );
    is::Button* button;
    if ( arg == 1 ) {
        button = new is::Button( luaL_checkstring( l, 1 ) );
    } else {
        button = new is::Button( luaL_checkstring( l, 1 ), luaL_checknumber( l, 2 ) );
    }
    gui->addNode( button );
    lua_pushnode( l, button );
    return 1;
}

int luaRegisterButtons( lua_State* l ) {
    // Create a function that puts a new is::Button on the stack.
    lua_register( l, "Button", luaCreateButton );
    return 0;
}
