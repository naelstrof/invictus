#include "../window.hpp"

int luaUpdateWindow( lua_State* l ) {
    window->m_changed = true;
    return 0;
}
