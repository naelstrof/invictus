// statemachine.hpp: System that handles changing game states.

#ifndef IS_STATEMACHINE_H_
#define IS_STATEMACHINE_H_

#include <string>
#include <vector>

#include "lua.hpp"

namespace is {

class State {
public:
    State( std::string name, int luaReference );
    ~State();
    void init();
    void deinit();
    void tick( float dt );
    std::string  m_name;
    int m_luaReference;
};

class StateMachine {
private:
    int m_activeState;
    std::vector<is::State*> m_states;
public:
    StateMachine();
    ~StateMachine();
    int     init();
    void    tick( float dt );
    void    setState( std::string statename );
    void    addState( is::State* );
    float   getTime();
};

};

void lua_pushState( lua_State* l, is::State* state );

extern is::StateMachine* states;

#endif // IS_STATEMACHINE_H_
