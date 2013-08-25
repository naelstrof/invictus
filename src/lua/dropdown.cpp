#include "../nodes/dropdown.hpp"

// Depends on ./lua/node.cpp
void lua_pushnode( lua_State* l, is::Node* node );
is::Node* lua_tonode( lua_State* l, int index );
is::Node* lua_checknode( lua_State* l, int index );

int luaDropdown__index( lua_State* l ) {
    is::Dropdown* dropdown = (is::Dropdown*)lua_tonode( l, 1 );
    std::string field = luaL_checkstring(l,2);
    if ( field == "onChange" ) {
        lua_rawgeti( l, LUA_REGISTRYINDEX, dropdown->m_luaChangeFunction );
        return 1;
    } else if ( field == "selection" ) {
        lua_pushnumber( l, dropdown->m_selectedElement );
        return 1;
    }
    return 0;
}

int luaDropdown__newindex( lua_State* l ) {
    is::Dropdown* dropdown = (is::Dropdown*)lua_tonode( l, 1 );
    std::string field = luaL_checkstring(l,2);
    if ( field == "onChange" ) {
        luaL_checktype( l, 3, LUA_TFUNCTION );
        lua_pushvalue( l, 3 );
        dropdown->m_luaChangeFunction = luaL_ref( lua->m_l, LUA_REGISTRYINDEX );
    } else if ( field == "selection" ) {
        dropdown->selectElement( luaL_checknumber( l, 3 ) );
    }
    return 0;
}

int luaDropdownAddRow( lua_State* l ) {
    is::Dropdown* dropdown = (is::Dropdown*)lua_tonode( l, 1 );
    lua_pushnumber( l, dropdown->addRow( luaL_checkstring( l , 2 ) ) );
    return 1;
}

int luaCreateDropdown( lua_State* l ) {
    int arg = lua_gettop( l );
    is::Dropdown* dropdown;
    if ( arg == 1 ) {
        dropdown = new is::Dropdown( luaL_checkstring( l, 1 ) );
    } else {
        dropdown = new is::Dropdown( luaL_checkstring( l, 1 ), luaL_checknumber( l, 2 ) );
    }
    gui->addNode( dropdown );
    lua_pushnode( l, dropdown );
    return 1;
}

int luaRegisterDropdowns( lua_State* l ) {
    // Create a function that puts a new is::Dropdown on the stack.
    lua_register( l, "Dropdown", luaCreateDropdown );

    luaL_getmetatable( l, "NodeBase" );
    // Register some new functions onto the node class
    static const luaL_Reg dropdownLib[] {
        { "addRow", luaDropdownAddRow },
        { NULL, NULL }
    };
    luaL_register( l, NULL, dropdownLib );
    return 0;
}
