#include <string>
#include "ParameterEntity.h"


namespace MAlice {
    
    ParameterEntity :: ParameterEntity (std::string identifier, int lineNumber, MAliceType type, bool passedByReference) : VariableEntity(identifier, lineNumber, type)
    {
      this->passedByReference = passedByReference;
    }

    ParameterEntity* ParameterEntity :: clone () {
      return new ParameterEntity(this->getIdentifier(), this->getLineNumber(), this->getType(), this->isPassedByReference());
    }
    
    std::string ParameterEntity::humanReadableName()
    {
        return "parameter";
    }

} 