#include "Regex.h"

Regex::Regex(std::string const regex)//Ex : a b + c . *
{

    std::vector<NFA> nfas;
    for(int i = 0; i < regex.size(); ++i)
    {
        char const& c = regex[i];
        switch(c)
        {
            default:
            {
                State * s = _stateFactory.state(static_cast<int>(c), {nullptr});
                std::vector<State**> vec = {&s->_out.back()};
                nfas.emplace_back(s, vec);
                break;
            }
            case '+':
            {
                NFA& e = nfas.back();
                nfas.pop_back();
                State * s = _stateFactory.state(-1, {e._in, nullptr});
                std::vector<State**> vec = {&s->_out.back()};
                connect(e, *s);
                nfas.emplace_back(e._in, vec);
                break;
            }
            case '*':
            {
                NFA& e = nfas.back();
                nfas.pop_back();
                State * s = _stateFactory.state(-1, {e._in, nullptr});
                connect(e, *s);
                std::vector<State**> vec = {&s->_out.back()};
                nfas.emplace_back(s, vec);
                break;
            }
            case '?':
            {
                NFA& e = nfas.back();
                nfas.pop_back();
                State * s = _stateFactory.state(-1, {e._in, nullptr});
                e._out.push_back(&s->_out[1]);
                nfas.emplace_back(s, e._out);
                break;
            }
            case '.':
            {
                NFA& e1 = nfas.back();
                nfas.pop_back();
                NFA& e2 = nfas.back();
                nfas.back();
                connect(e1, *e2._in);
                nfas.emplace_back(e1._in, e2._out);
                break;
            }
            case '|':
            {
                NFA& e1 = nfas.back();
                nfas.pop_back();
                NFA& e2 = nfas.back();
                nfas.back();
                State * s = _stateFactory.state(-1, {e1._in, e2._in});
                e1._out.insert(e1._out.end(), e2._out.begin(), e2._out.end());
                nfas.emplace_back(s, e1._out);
                break;  
            }
        }
    }
    NFA& e = nfas.back();
    nfas.pop_back();
    State * matchstate = _stateFactory.state(-2, {});
    connect(e, *matchstate);
    _in = e._in;
}

bool Regex::match(std::string const sentence)
{
    for(int i = 0; i < sentence.size(); ++i)
    {


    }
    return true;
}