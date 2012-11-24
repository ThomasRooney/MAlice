
#ifndef _MALICEVARIABLEENTITY
#define _MALICEVARIABLEENTITY

#include <iostream>

#include "Entity.h"
#include "Types.h"

namespace MAlice {

class VariableEntity : public Entity {

private:
    MAliceType m_type;
    
public:
    VariableEntity(std::string identifier, unsigned int lineNumber, MAliceType type);
    
    std::string humanReadableName();
    MAliceType getType();


    
}; // class VariableEntity
    
}; // namespace MAlice

#endif
