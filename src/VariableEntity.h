
#ifndef _MALICEVARIABLEENTITY
#define _MALICEVARIABLEENTITY

#include <iostream>

#include "Entity.h"

namespace MAlice {

class VariableEntity : public Entity {

private:
    unsigned short m_type;
    
public:
    VariableEntity(std::string identifier, unsigned int lineNumber, unsigned short type);
    
    std::string humanReadableName();
    
}; // class VariableEntity
    
}; // namespace MAlice

#endif
