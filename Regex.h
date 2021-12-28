#ifndef __REGEX__
#define __REGEX__

#include <string>
#include "NFA.h"

class Regex
{
    StateFactory _stateFactory;
    State * _in;//Represents the input of the NFA
    
    public:
    Regex(std::string const regex);
    bool match(std::string const sentence);
};

#endif