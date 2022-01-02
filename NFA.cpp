#include "NFA.h"

State::State(int c, std::vector<State*> out) : _last_iteration(-1), _c(c), _out(out)
{

}

State * StateFactory::state(int c, std::vector<State*> out)
{
    _states.push_back(new State(c, out));
    return _states.back();
}

StateFactory::~StateFactory()
{
    for(std::vector<State*>::iterator it = _states.begin(); it != _states.end(); ++it)
    {
        delete *it;
    }
}

NFA::NFA(State * in, std::vector<State **> out) : _in(in), _out(out)
{

}

void connect(NFA & nfa, State & state)
{
    for(std::vector<State**>::iterator it = nfa._out.begin(); it != nfa._out.end(); ++it)
    {
        State **& ptr = *it;
        *ptr = &state;
    }
}