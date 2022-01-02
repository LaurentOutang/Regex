#ifndef __NFA__
#define __NFA__
#include <vector>

class State
{
    public:
    int _last_iteration;
    private:
    int _c;
    std::vector<State *> _out;//>1 if split
    
    State(int c = -1, std::vector<State*> out = {});

    friend class Regex;
    friend class StateFactory;
};

class StateFactory
{
    private:
    std::vector<State *> _states;
    public:
    StateFactory() = default;
    ~StateFactory();
    State * state(int c = -1, std::vector<State*> out = {});
};

class NFA
{
    State * _in;
    std::vector<State **> _out;

    public:
    NFA(State * in = nullptr, std::vector<State **> out = {});

    friend class Regex;
    friend void connect(NFA & nfa, State & state);
};

void connect(NFA & nfa, State & state);

#endif