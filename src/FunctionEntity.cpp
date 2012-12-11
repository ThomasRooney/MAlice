
#include "FunctionEntity.h"
#include "Type.h"

using namespace std;

namespace MAlice {

    FunctionEntity::FunctionEntity(string identifier, unsigned int lineNumber, vector<ParameterEntity*> parameterListTypes, Type returnType) : FunctionProcedureEntity(identifier, lineNumber, parameterListTypes), m_returnType(returnType)
    {
    }
    
    std::string FunctionEntity::humanReadableName()
    {
        return "function";
    }
    
    FunctionEntity *FunctionEntity::clone()
    {
        return new FunctionEntity(getIdentifier(), getLineNumber(), getParameterListTypes(), getReturnType());
    }

};