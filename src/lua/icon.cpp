#include "../nodes/icon.hpp"

// Depends on ./lua/node.cpp

int luaCreateIcon( lua_State* l ) {
    int arg = lua_gettop( l );
    is::Icon* icon;
    if ( arg == 1 ) {
        icon = new is::Icon( luaL_checkstring( l, 1 ) );
    } else {
        icon = new is::Icon( luaL_checkstring( l, 1 ), luaL_checknumber( l, 2 ) );
    }
    gui->addNode( icon );
    lua_pushnode( l, icon );
    return 1;
}

int luaRegisterIcons( lua_State* l ) {
    // Create a function that puts a new is::Icon on the stack.
    lua_register( l, "Icon", luaCreateIcon );
    return 0;
}
