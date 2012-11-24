
#include "ProcedureEntity.h"

namespace MAlice {

    ProcedureEntity::ProcedureEntity(std::string identifier, unsigned int lineNumber, std::list<MAliceType> parameterListTypes) : FunctionProcedureEntity(identifier, lineNumber, parameterListTypes)
    {
    }
    
    std::string ProcedureEntity::humanReadableName()
    {
        return "procedure";
    }

}; // namespace MAlice