#include "../nodes/button.hpp"

// Depends on ./lua/node.cpp

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
