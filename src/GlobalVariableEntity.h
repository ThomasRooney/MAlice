
#ifndef _MALICEGLOBALVARIABLEENTITY
#define _MALICEGLOBALVARIABLEENTITY

#include <iostream>

#include "VariableEntity.h"
#include "Types.h"

namespace MAlice {
    
    class GlobalVariableEntity : public VariableEntity {
    public:
        GlobalVariableEntity(std::string identifier, unsigned int lineNumber, Type type);
    }; //class GlobalVariableEntity
    
} // namespace MAlice

#endif // #ifndef _MALICEGLOBALVARIABLEENTITY
