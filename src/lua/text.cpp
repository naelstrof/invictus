#include "../nodes/text.hpp"

int luaTextSet( lua_State* l ) {
    // TODO: Check and make sure it's actually a text node before attempting to access the member :u.
    is::Text* textnode = (is::Text*)lua_checknode( l, 1 );
    std::string text = luaL_checkstring( l, 2 );
    textnode->setText( text );
    return 0;
}

int luaCreateText( lua_State* l ) {
    is::Text* text = new is::Text( luaL_checkstring( l, 1 ) );
    gui->addNode( text );
    lua_pushnode( l, text );
    return 1;
}

int luaRegisterTexts( lua_State* l ) {
    // Get the node metatable and add some text features to it.
    luaL_getmetatable( l, "NodeBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg textLib[] {
        { "setText", luaTextSet },
        { NULL, NULL }
    };
    luaL_register( l, NULL, textLib );
    // Clear the stack
    lua_pop(l,1);

    // Create a function that puts a new is::Text on the stack.
    lua_register( l, "Text", luaCreateText );
    return 0;
}
