
#include "FunctionEntity.h"

using namespace std;

namespace MAlice {

FunctionEntity::FunctionEntity(string identifier, list<unsigned short> parameterListTypes, unsigned short returnType) : FunctionProcedureEntity(identifier, parameterListTypes)
{
    m_returnType = returnType;
}

};