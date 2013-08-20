#include "../nodes/icon.hpp"

is::Node* lua_tonode( lua_State* l, int index ) {
    is::Node** node = (is::Node**)luaL_checkudata( l, index, "NodeBase" );
    return *node;
}

is::Node* lua_checknode( lua_State* l, int narg )
{
    is::Node* foo = lua_tonode( l, narg );
    if (foo == NULL) {
        luaL_argerror( l, narg, "attempt to index a NULL Node!" );
    }
    return foo;
}

void lua_pushnode( lua_State* l, is::Node* node )
{
    is::Node** pointer = (is::Node**)lua_newuserdata( l, sizeof(is::Node*) );
    *pointer = node;
    luaL_getmetatable( l, "NodeBase" );
    lua_setmetatable( l,-2 );
}

int luaNode__gc( lua_State* l ) {
    is::Node* node = lua_checknode( l, 1 );
    gui->remove( node );
    return 0;
}

int luaNodeRemove( lua_State* l ) {
    is::Node* node = lua_checknode( l, 1 );
    gui->remove( node );
    return 0;
}

int luaNode__index( lua_State* l ) {
    is::Node* node = lua_tonode(l,1);
    if ( node == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Node!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "pos" ) {
        lua_pushvector( l, node->getPos() );
        return 1;
    } else if ( field == "ang" ) {
        lua_pushvector( l, node->getAng() );
        return 1;
    } else if ( field == "color" ) {
        lua_pushcolor( l, node->getColor() );
        return 1;
    } else if ( field == "x" ) {
        lua_pushnumber( l, node->getPos().x );
        return 1;
    } else if ( field == "y" ) {
        lua_pushnumber( l, node->getPos().y );
        return 1;
    } else if ( field == "z" ) {
        lua_pushnumber( l, node->getPos().z );
        return 1;
    } else if ( field == "r" ) {
        lua_pushnumber( l, node->getColor().x );
        return 1;
    } else if ( field == "g" ) {
        lua_pushnumber( l, node->getColor().y );
        return 1;
    } else if ( field == "b" ) {
        lua_pushnumber( l, node->getColor().z );
        return 1;
    } else if ( field == "a" ) {
        lua_pushnumber( l, node->getColor().w );
        return 1;
    }
    lua_rawgeti( l, LUA_REGISTRYINDEX, node->m_luaReference );
    lua_pushvalue( l, 2 );
    lua_gettable( l, -2 );
    return 1;
}

int luaNode__newindex(lua_State* l) {
    is::Node* node = lua_tonode(l,1);
    if ( node == NULL ) {
        lua_Debug ar1;
        lua_getstack( l, 1, &ar1 );
        lua_getinfo( l, "fl", &ar1 );
        lua_Debug ar2;
        lua_getinfo( l, ">S", &ar2 );
        lua_pushfstring( l, "%s:%d: attempt to index a NULL Node!", ar2.short_src, ar1.currentline );
        return lua_error( l );
    }
    std::string field = luaL_checkstring(l,2);
    if ( field == "pos" ) {
        node->setPos( *lua_checkvector( l, 3 ) );
    } else if ( field ==  "ang" ) {
        node->setAng( *lua_checkvector( l, 3 ) );
    } else if ( field ==  "color" ) {
        node->setColor( *lua_checkcolor( l, 3 ) );
    } else if ( field ==  "x" ) {
        glm::vec3 pos = node->getPos();
        pos.x = luaL_checknumber( l, 3 );
        node->setPos( pos );
    } else if ( field ==  "y" ) {
        glm::vec3 pos = node->getPos();
        pos.y = luaL_checknumber( l, 3 );
        node->setPos( pos );
    } else if ( field ==  "z" ) {
        glm::vec3 pos = node->getPos();
        pos.z = luaL_checknumber( l, 3 );
        node->setPos( pos );
    } else if ( field ==  "r" ) {
        glm::vec4 color = node->getColor();
        color.x = luaL_checknumber( l, 3 );
        node->setColor( color );
    } else if ( field ==  "g" ) {
        glm::vec4 color = node->getColor();
        color.y = luaL_checknumber( l, 3 );
        node->setColor( color );
    } else if ( field ==  "b" ) {
        glm::vec4 color = node->getColor();
        color.z = luaL_checknumber( l, 3 );
        node->setColor( color );
    } else if ( field ==  "a" ) {
        glm::vec4 color = node->getColor();
        color.w = luaL_checknumber( l, 3 );
        node->setColor( color );
    } else {
        if ( node->m_luaReference == LUA_NOREF ) {
            lua_newtable( l );
            node->m_luaReference = luaL_ref( l, LUA_REGISTRYINDEX );
        }
        lua_rawgeti( l, LUA_REGISTRYINDEX, node->m_luaReference );
        lua_pushvalue( l, 3 );
        lua_setfield( l, -2, luaL_checkstring( l, 2 ) );
    }

    return 0;
}

int luaRegisterNodes( lua_State* l ) {
    // Create a new table called NodeBase
    luaL_newmetatable( l, "NodeBase" );
    // Register its new index function and garbage collection function
    static const luaL_Reg nodeLib[] {
        { "remove", luaNodeRemove },
        { "__index", luaNode__index },
        { "__newindex", luaNode__newindex },
        { "__gc", luaNode__gc },
        { NULL, NULL }
    };
    luaL_register( l, NULL, nodeLib );
    // Set its type to Node
    lua_pushstring( l, "Node" );
    lua_setfield( l, -2, "__type" );
    // Clear the stack
    lua_pop(l,1);

    return 0;
}
