#include "../nodes/icon.hpp"

// Depends on ./lua/node.cpp

int luaCreateIcon( lua_State* l ) {
    is::Icon* icon = new is::Icon( luaL_checkstring( l, 1 ) );
    gui->addNode( icon );
    lua_pushnode( l, icon );
    return 1;
}

int luaRegisterIcons( lua_State* l ) {
    // Create a function that puts a new is::Icon on the stack.
    lua_register( l, "Icon", luaCreateIcon );
    return 0;
}
