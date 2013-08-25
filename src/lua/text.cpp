#include "../nodes/text.hpp"

void lua_pushnode( lua_State* l, is::Node* node );
is::Node* lua_tonode( lua_State* l, int index );
is::Node* lua_checknode( lua_State* l, int index );

// This stuff plugs into node's __index and __newindex; since I couldn't get it to work through lua.
int luaText__index( lua_State* l ) {
    is::Text* text = (is::Text*)lua_tonode(l,1);
    std::string field = luaL_checkstring(l,2);
    if ( field == "size" ) {
        lua_pushnumber( l, text->m_size );
        return 1;
    } else if ( field == "text" ) {
        lua_pushstring( l, text->getText().c_str() );
        return 1;
    } else if ( field == "width" ) {
        text->getDimensions();
        lua_pushnumber( l, text->m_width );
        return 1;
    } else if ( field == "height" ) {
        text->getDimensions();
        lua_pushnumber( l, text->m_height );
        return 1;
    }

    return 0;
}

int luaText__newindex( lua_State* l ) {
    is::Text* text = (is::Text*)lua_tonode(l,1);
    std::string field = luaL_checkstring(l,2);
    if ( field == "size" ) {
        text->setSize( luaL_checknumber( l, 3 ) );
    } else if ( field == "text" ) {
        text->setText( luaL_checkstring( l, 3 ) );
    }
    return 0;
}

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
