#include <string>
#include "ParameterEntity.h"


namespace MAlice {
  ParameterEntity :: ParameterEntity (std::string identifier, int lineNumber, MAliceType type, bool passedByReference) : VariableEntity(identifier, lineNumber, type)
  {
      
  }

  ParameterEntity* ParameterEntity :: clone () {
      return new ParameterEntity(this->getIdentifier(), this->getLineNumber(), this->getType(), this->isPassedByReference());
  }

} 