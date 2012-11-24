
#include "FunctionEntity.h"

using namespace std;

namespace MAlice {

    FunctionEntity::FunctionEntity(string identifier, unsigned int lineNumber, list<ParameterEntity> parameterListTypes, MAliceType returnType) : FunctionProcedureEntity(identifier, lineNumber, parameterListTypes)
    {
        m_returnType = returnType;
    }
    
    std::string FunctionEntity::humanReadableName()
    {
        return "function";
    }

};