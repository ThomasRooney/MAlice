#include <string>
#include "ParameterEntity.h"


namespace MAlice {
    
    ParameterEntity :: ParameterEntity (std::string identifier, int lineNumber, Type type) : VariableEntity(identifier, lineNumber, type)
    {
        m_byReference = false;
    }
    
    ParameterEntity* ParameterEntity :: clone () {
      return new ParameterEntity(this->getIdentifier(), this->getLineNumber(), this->getType());
    }
    
    std::string ParameterEntity::humanReadableName()
    {
        return "parameter";
    }
    
    bool ParameterEntity::getByReference()
    {
        return m_byReference;
    }
    
    void ParameterEntity::setByReference(bool byReference)
    {
        m_byReference = byReference;
    }

}