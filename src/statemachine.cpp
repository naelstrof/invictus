#include "statemachine.hpp"

is::StateMachine* states = new is::StateMachine();

is::StateMachine::StateMachine() {
    // Start without an active state.
    m_activeState = -1;
}

is::StateMachine::~StateMachine() {
    for ( unsigned int i=0; i<m_states.size(); i++ ) {
        delete m_states[i];
    }
}

int is::StateMachine::init() {
    if ( m_states.size() > 0 && m_activeState == -1 ) {
        m_activeState = 0;
        m_states.at( m_activeState )->init();
    }
    return 0;
}

void is::StateMachine::setState( std::string statename ) {
    for ( unsigned int i=0; i<m_states.size(); i++ ) {
        if ( m_states[i]->m_name == statename ) {
            if ( i == ( unsigned int )m_activeState) {
                return;
            }

            if ( m_activeState != -1 ) {
                m_states.at( m_activeState )->deinit();
            }

            m_activeState = i;
            m_states.at( m_activeState )->init();

        }
    }
}

void is::StateMachine::addState( is::State* state ) {
    m_states.push_back( state );
    if ( m_activeState == -1 ) {
        m_activeState = m_states.size()-1;
    }
}

void is::StateMachine::tick( float dt ) {
    if ( m_activeState == -1 ) {
        return;
    }
    m_states.at( m_activeState )->tick( dt );
}

is::State::State( std::string name, int luaReference )
    : m_name( name ), m_luaReference( luaReference ) {
}

is::State::~State() {
    luaL_unref( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
}

void is::State::init() {
    // FIXME: This check might be entirely useless.
    if ( m_luaReference == LUA_NOREF ) {
        return;
    }

    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
    lua_getfield( lua->m_l, -1, "onInit" );
    if ( !lua_isfunction( lua->m_l, -1 ) ) {
        lua_pop( lua->m_l, 2 );
        return;
    }
    lua_pushState( lua->m_l, this );
    lua->call( 1, 0 );
    lua_pop( lua->m_l, 1 );
}

void is::State::deinit() {
    if ( m_luaReference == LUA_NOREF ) {
        return;
    }

    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
    lua_getfield( lua->m_l, -1, "onExit" );
    if ( !lua_isfunction( lua->m_l, -1 ) ) {
        lua_pop( lua->m_l, 2 );
        return;
    }
    lua_pushState( lua->m_l, this );
    lua->call( 1, 0 );
    lua_pop( lua->m_l, 1 );
}

void is::State::tick( float dt ) {
    if ( m_luaReference == LUA_NOREF ) {
        return;
    }

    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
    lua_getfield( lua->m_l, -1, "onTick" );
    if ( !lua_isfunction( lua->m_l, -1 ) ) {
        lua_pop( lua->m_l, 2 );
        return;
    }
    lua_pushState( lua->m_l, this );
    lua_pushnumber( lua->m_l, dt );
    lua->call( 2, 0 );
    lua_pop( lua->m_l, 1 );
}
