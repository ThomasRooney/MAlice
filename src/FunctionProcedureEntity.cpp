
#include "FunctionProcedureEntity.h"

namespace MAlice {

    FunctionProcedureEntity::FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::vector<ParameterEntity*> parameterListTypes) : Entity(identifier, lineNumber)
    {
        m_parameterListTypes = parameterListTypes;
    }
    
    void FunctionProcedureEntity::setParameterListTypes(std::vector<ParameterEntity*> types)
    {
        m_parameterListTypes = types;
    }
    
    std::vector<ParameterEntity*> FunctionProcedureEntity::getParameterListTypes()
    {
        return m_parameterListTypes;
    }

}; // namespace MAlice