#include "../nodes/text.hpp"

// Depends on ../lua/node.cpp

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
    // Create a function that puts a new is::Text on the stack.
    lua_register( l, "Text", luaCreateText );
    return 0;
}
