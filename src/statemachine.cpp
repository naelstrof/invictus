#include "statemachine.hpp"

is::StateMachine* states = new is::StateMachine();

is::StateMachine::StateMachine() {
    // Start without an active state.
    m_activeState = -1;
    m_nextState = -1;
}

is::StateMachine::~StateMachine() {
    for ( unsigned int i=0; i<m_states.size(); i++ ) {
        delete m_states[i];
    }
}

int is::StateMachine::init() {
    return 0;
}

is::State* is::StateMachine::getCurrentState() {
    return m_states.at( m_activeState );
}

void is::StateMachine::setState( std::string statename ) {
    for ( unsigned int i=0; i<m_states.size(); i++ ) {
        if ( m_states.at(i)->m_name == statename ) {
            if ( i == ( unsigned int )m_activeState) {
                return;
            }
            m_nextState = i;
            return;
        }
    }
    os->printf( "ERR Couldn't find state named %!\n", statename );
}

void is::StateMachine::addState( is::State* state ) {
    m_states.push_back( state );
}

void is::StateMachine::tick( float dt ) {
    if ( m_nextState != -1 ) {
        if ( m_activeState != -1 ) {
            m_states.at( m_activeState )->deinit();
        }

        m_activeState = m_nextState;
        m_nextState = -1;
        m_states.at( m_activeState )->init();
    }
    if ( m_activeState == -1 ) {
        return;
    }
    m_states.at( m_activeState )->tick( dt );
}

is::State::State( std::string name, int luaReference )
    : m_name( name ), m_luaReference( luaReference ) {
    m_luaStateReference = LUA_NOREF;
}

is::State::~State() {
    luaL_unref( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
    if ( m_luaStateReference != LUA_NOREF ) {
        luaL_unref( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
    }

    for ( unsigned int i=0; i<m_timers.size(); i++ ) {
        delete m_timers.at( i );
    }
}

void is::State::addTimer( int luaFunction, float timeDiff ) {
    m_timers.push_back( new is::Timer( luaFunction, timeDiff ) );
}

void is::State::init() {
    if ( m_luaStateReference == LUA_NOREF ) {
        lua_pushstate( lua->m_l, this );
        m_luaStateReference = luaL_ref( lua->m_l, LUA_REGISTRYINDEX );
    }

    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
    lua_getfield( lua->m_l, -1, "onInit" );
    if ( !lua_isfunction( lua->m_l, -1 ) ) {
        lua_pop( lua->m_l, 2 );
        return;
    }
    //lua_pushstate( lua->m_l, this );
    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaStateReference );
    lua->call( lua->m_l, 1, 0 );
    lua_pop( lua->m_l, 1 );
}

void is::State::deinit() {
    if ( m_luaStateReference == LUA_NOREF ) {
        lua_pushstate( lua->m_l, this );
        m_luaStateReference = luaL_ref( lua->m_l, LUA_REGISTRYINDEX );
    }

    for ( unsigned int i=0; i<m_timers.size(); i++ ) {
        delete m_timers.at( i );
    }
    m_timers.clear();

    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
    lua_getfield( lua->m_l, -1, "onExit" );
    if ( !lua_isfunction( lua->m_l, -1 ) ) {
        lua_pop( lua->m_l, 2 );
        return;
    }
    //lua_pushstate( lua->m_l, this );
    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaStateReference );
    lua->call( lua->m_l, 1, 0 );
    lua_pop( lua->m_l, 1 );
}

void is::State::tick( float dt ) {
    if ( m_luaStateReference == LUA_NOREF ) {
        lua_pushstate( lua->m_l, this );
        m_luaStateReference = luaL_ref( lua->m_l, LUA_REGISTRYINDEX );
    }

    float curtime = os->getElapsedTime();
    for ( int i=0; i<(int)m_timers.size(); i++ ) {
        if ( curtime > m_timers.at(i)->m_timeTrigger ) {
            lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_timers.at(i)->m_luaFunction );
            if ( !lua_isfunction( lua->m_l, -1 ) ) {
                lua_pop( lua->m_l, 1 );
                delete m_timers.at(i);
                m_timers.erase( m_timers.begin() + i );
                if ( i > 0 ) {
                    i--;
                }
                continue;
            }
            lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaStateReference );
            lua->call( lua->m_l, 1, 0 );
            delete m_timers.at(i);
            m_timers.erase( m_timers.begin() + i );
            if ( i > 0 ) {
                i--;
            }
        }
    }

    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaReference );
    lua_getfield( lua->m_l, -1, "onTick" );
    if ( !lua_isfunction( lua->m_l, -1 ) ) {
        lua_pop( lua->m_l, 2 );
        return;
    }
    //lua_pushstate( lua->m_l, this );
    lua_rawgeti( lua->m_l, LUA_REGISTRYINDEX, m_luaStateReference );
    lua_pushnumber( lua->m_l, dt );
    lua->call( lua->m_l, 2, 0 );
    lua_pop( lua->m_l, 1 );
}

is::Timer::Timer( int luaFunction, float timeDiff ) {
    m_luaFunction = luaFunction;
    m_timeTrigger = os->getElapsedTime()+timeDiff;
}

is::Timer::~Timer() {
    luaL_unref( lua->m_l, LUA_REGISTRYINDEX, m_luaFunction );
}
