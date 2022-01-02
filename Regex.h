#ifndef __REGEX__
#define __REGEX__

#include <string>
#include <functional>
#include "NFA.h"

class Regex
{
    StateFactory _stateFactory;
    State * _in;//Represents the input of the NFA
    
    public:
    Regex(std::string const regex);//Assuming regex is a valid postfix regex
    bool match(std::string const sentence);
};

#endif