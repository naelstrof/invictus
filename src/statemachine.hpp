// statemachine.hpp: System that handles changing game states.

#ifndef IS_STATEMACHINE_H_
#define IS_STATEMACHINE_H_

#include <string>
#include <vector>

#include "lua.hpp"
#include "os.hpp"

namespace is {

class Timer {
public:
    Timer( int luaFunctionReference, float timeDiff );
    ~Timer();
    int m_luaFunction;
    float m_timeTrigger;
};

class State {
public:
    State( std::string name, int luaReference );
    ~State();
    void init();
    void deinit();
    void addTimer( int luaFunction, float timeDiff );
    void tick( float dt );
    std::string  m_name;
    int m_luaReference;
    int m_luaStateReference;
    std::vector<is::Timer*> m_timers;
};

class StateMachine {
private:
    int m_activeState;
    std::vector<is::State*> m_states;
public:
    StateMachine();
    ~StateMachine();
    int        init();
    void       tick( float dt );
    void       setState( std::string statename );
    void       addState( is::State* );
    is::State* getCurrentState();
};

};

void lua_pushstate( lua_State* l, is::State* state );

extern is::StateMachine* states;

#endif // IS_STATEMACHINE_H_
