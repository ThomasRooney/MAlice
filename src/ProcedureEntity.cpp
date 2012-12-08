
#include "ProcedureEntity.h"

namespace MAlice {

    ProcedureEntity::ProcedureEntity(std::string identifier, unsigned int lineNumber, std::vector<ParameterEntity*> parameterListTypes) : FunctionProcedureEntity(identifier, lineNumber, parameterListTypes)
    {
    }
    
    std::string ProcedureEntity::humanReadableName()
    {
        return "procedure";
    }
    
    ProcedureEntity *ProcedureEntity::clone()
    {
        return new ProcedureEntity(getIdentifier(), getLineNumber(), getParameterListTypes());
    }

}; // namespace MAlice