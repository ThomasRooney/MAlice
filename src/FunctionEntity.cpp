
#include "FunctionEntity.h"

using namespace std;

namespace MAlice {

    FunctionEntity::FunctionEntity(string identifier, unsigned int lineNumber, list<unsigned short> parameterListTypes, unsigned short returnType) : FunctionProcedureEntity(identifier, lineNumber, parameterListTypes)
    {
        m_returnType = returnType;
    }
    
    std::string FunctionEntity::humanReadableName()
    {
        return "function";
    }

};