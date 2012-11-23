
#ifndef _MALICEVARIABLEENTITY
#define _MALICEVARIABLEENTITY

#include <iostream>

#include "Entity.h"

namespace MAlice {

class VariableEntity : Entity {

private:
    unsigned short m_type;
    
public:
    VariableEntity(std::string identifier, unsigned short type);
    
}; // class VariableEntity
    
}; // namespace MAlice

#endif
