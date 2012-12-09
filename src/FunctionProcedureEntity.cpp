
#include "FunctionProcedureEntity.h"

namespace MAlice {

    FunctionProcedureEntity::FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::vector<ParameterEntity*> parameterListTypes) : Entity(identifier, lineNumber)
    {
        m_parameterListTypes = parameterListTypes;
        m_LLVMFunction = NULL;
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