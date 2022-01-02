#include "Regex.h"

Regex::Regex(std::string const regex)
{

    std::vector<NFA *> nfasOwner;
    std::vector<NFA *> nfas;
    bool escaped = false;
    for(int i = 0; i < regex.size(); ++i)
    {
        char const& c = regex[i];
        if(escaped)//Escape the current character
        {
            State * s = _stateFactory.state(static_cast<int>(c), {nullptr});
            std::vector<State**> vec = {&s->_out.back()};
            nfasOwner.push_back(new NFA(s, vec));
            nfas.push_back(nfasOwner.back());
            escaped = false;
        }
        else{
            switch(c)
            {
                default:
                {
                    State * s = _stateFactory.state(static_cast<int>(c), {nullptr});
                    std::vector<State**> vec = {&s->_out.back()};
                    nfasOwner.push_back(new NFA(s, vec));
                    nfas.push_back(nfasOwner.back());
                    break;
                }
                case '+':
                {
                    NFA& e = *nfas.back();
                    nfas.pop_back();
                    State * s = _stateFactory.state(-1, {e._in, nullptr});
                    std::vector<State**> vec = {&s->_out.back()};
                    connect(e, *s);
                    nfasOwner.push_back(new NFA(e._in, vec));
                    nfas.push_back(nfasOwner.back());
                    break;
                }
                case '*':
                {
                    NFA& e = *nfas.back();
                    nfas.pop_back();
                    State * s = _stateFactory.state(-1, {e._in, nullptr});
                    connect(e, *s);
                    std::vector<State**> vec = {&s->_out.back()};
                    nfasOwner.push_back(new NFA(s, vec));
                    nfas.push_back(nfasOwner.back());
                    break;
                }
                case '?':
                {
                    NFA& e = *nfas.back();
                    nfas.pop_back();
                    State * s = _stateFactory.state(-1, {e._in, nullptr});
                    e._out.push_back(&s->_out[1]);
                    nfasOwner.push_back(new NFA(s, e._out));
                    nfas.push_back(nfasOwner.back());
                    break;
                }
                case '.':
                {
                    NFA& e1 = *nfas.back();
                    nfas.pop_back();
                    NFA& e2 = *nfas.back();
                    nfas.pop_back();
                    connect(e2, *e1._in);
                    nfasOwner.push_back(new NFA(e2._in, e1._out));
                    nfas.push_back(nfasOwner.back());
                    break;
                }
                case '|':
                {
                    NFA& e1 = *nfas.back();
                    nfas.pop_back();
                    NFA& e2 = *nfas.back();
                    nfas.pop_back();
                    State * s = _stateFactory.state(-1, {e1._in, e2._in});
                    e1._out.insert(e1._out.end(), e2._out.begin(), e2._out.end());
                    nfasOwner.push_back(new NFA(s, e1._out));
                    nfas.push_back(nfasOwner.back());
                    break;  
                }
                case '\\':
                {
                    if(!escaped)
                        escaped = true;
                }
            }
        }
        
    }
    NFA& e = *nfas.back();
    nfas.pop_back();
    State * matchstate = _stateFactory.state(-2, {});
    connect(e, *matchstate);
    _in = e._in;

    for(std::vector<NFA*>::iterator it = nfasOwner.begin(); it != nfasOwner.end(); ++it)
        delete *it;
}

bool Regex::match(std::string const sentence)
{
    std::vector<State*> currentStates = {_in};
    std::vector<State*> nextStates;

    int i = 0;
    std::function<void(State*)> addState;
    addState = [&addState, &nextStates, i](State* s) {
        if(s == nullptr || s->_last_iteration == i)
            return;
        if(s->_c == -1)
        {
            for(int j = 0; j < s->_out.size(); ++j)
            {
                addState(s->_out[j]);
            }
            return;
        }
        nextStates.push_back(s);
    };

    if(currentStates[0]->_c == -1)
    {
        for(int k = 0; k < currentStates[0]->_out.size(); ++k)
        {
            addState(currentStates[0]->_out[k]);
        }
        currentStates.swap(nextStates);
        nextStates.clear();
    }   

    for(; i < sentence.size(); ++i)
    {
        int c = static_cast<int>(sentence[i]);
        for(int j = 0; j < currentStates.size(); ++j)//Tester tous les états courrants
        {
            if(currentStates[j]->_c == c)
                addState(currentStates[j]->_out[0]);
        }
        currentStates.swap(nextStates);
        nextStates.clear();
    }

    //Once the whole sentence is read, check if final state
    i = 0;
    bool terminalStateFound = false;
    while(i < currentStates.size() && !terminalStateFound)
    {
        if(currentStates[i]->_c == -2)
            terminalStateFound = true;
        ++i;
    }
    return terminalStateFound;
}