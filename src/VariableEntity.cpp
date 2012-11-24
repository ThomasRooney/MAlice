
#include "VariableEntity.h"

namespace MAlice {

    VariableEntity::VariableEntity(std::string identifier, unsigned int lineNumber, MAliceType type) : Entity(identifier, lineNumber)
    {
        m_type = type;
    }
    
    std::string VariableEntity::humanReadableName()
    {
        return "variable";
    }

};