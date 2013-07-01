int luaPrint( lua_State* l ) {
    // Get argument count.
    int argnum = lua_gettop( l );
    // Push the 'tostring' function to the stack.
    lua_getglobal( l, "tostring" );
    // Prepend the message with LUA so user knows the source.
    std::string message = "LUA ";
    for ( int i=1; i<=argnum; i++ ) {
        const char *s;
        // Re-push the function to the stack.
        lua_pushvalue( l, -1 );
        // Then the variable supplied to print() as an arg.
        lua_pushvalue( l, i );
        // Finally call the function. (Which pops it all from the stack)
        lua_call( l, 1, 1 );
        // Convert the result to a C string.
        s = lua_tostring( l, -1 );
        if ( s == NULL ) {
            return luaL_error( l, LUA_QL( "tostring" ) " must return a string to " LUA_QL( "print" ) );
        }
        if ( i > 1 ) {
            message.append( "\t" );
        }
        message.append( s );
        // Finally pop the string from the stack.
        lua_pop( l, 1 );
    }
    os->printf( "%\n", message );
    return 0;
}
