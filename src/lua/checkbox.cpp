#include "../nodes/checkbox.hpp"

// Depends on ./lua/node.cpp
void lua_pushnode( lua_State* l, is::Node* node );
is::Node* lua_tonode( lua_State* l, int index );
is::Node* lua_checknode( lua_State* l, int index );

int luaCheckbox__index( lua_State* l ) {
    is::Checkbox* checkbox = (is::Checkbox*)lua_tonode(l,1);
    std::string field = luaL_checkstring(l,2);
    if ( field == "onCheck" ) {
        lua_rawgeti( l, LUA_REGISTRYINDEX, checkbox->m_luaCheckFunction );
        return 1;
    } else if ( field == "onUncheck" ) {
        lua_rawgeti( l, LUA_REGISTRYINDEX, checkbox->m_luaUncheckFunction );
        return 1;
    } else if ( field == "checked" ) {
        lua_pushboolean( l, checkbox->m_checked );
        return 1;
    }
    return 0;
}

int luaCheckbox__newindex( lua_State* l ) {
    is::Checkbox* checkbox = (is::Checkbox*)lua_tonode(l,1);
    std::string field = luaL_checkstring(l,2);
    if ( field == "onCheck" ) {
        luaL_checktype( l, 3, LUA_TFUNCTION );
        lua_pushvalue( l, 3 );
        checkbox->m_luaCheckFunction = luaL_ref( lua->m_l, LUA_REGISTRYINDEX );
    } else if ( field == "onUncheck" ) {
        luaL_checktype( l, 3, LUA_TFUNCTION );
        lua_pushvalue( l, 3 );
        checkbox->m_luaUncheckFunction = luaL_ref( lua->m_l, LUA_REGISTRYINDEX );
    } else if ( field == "checked" ) {
        checkbox->m_checked = lua_toboolean( l, 3 );
    }
    return 0;
}

int luaCreateCheckbox( lua_State* l ) {
    int arg = lua_gettop( l );
    is::Checkbox* checkbox;
    if ( arg == 1 ) {
        checkbox = new is::Checkbox( luaL_checkstring( l, 1 ) );
    } else if ( arg == 2 ) {
        checkbox = new is::Checkbox( luaL_checkstring( l, 1 ), luaL_checknumber( l, 2 ) );
    } else {
        checkbox = new is::Checkbox( luaL_checkstring( l, 1 ), luaL_checknumber( l, 2 ), lua_toboolean( l, 3 ) );
    }
    gui->addNode( checkbox );
    lua_pushnode( l, checkbox );
    return 1;
}

int luaRegisterCheckboxs( lua_State* l ) {
    // Create a function that puts a new is::Checkbox on the stack.
    lua_register( l, "Checkbox", luaCreateCheckbox );
    return 0;
}
