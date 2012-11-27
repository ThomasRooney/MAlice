
#include "FunctionProcedureEntity.h"

namespace MAlice {

    FunctionProcedureEntity::FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::list<ParameterEntity> parameterListTypes) : Entity(identifier, lineNumber)
    {
        m_parameterListTypes = parameterListTypes;
    }
    
    void FunctionProcedureEntity::setParameterListTypes(std::list<ParameterEntity> types)
    {
        m_parameterListTypes = types;
    }
    
    std::list<ParameterEntity> FunctionProcedureEntity::getParameterListTypes()
    {
        return m_parameterListTypes;
    }

}; // namespace MAlice