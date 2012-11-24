
#include "FunctionProcedureEntity.h"

namespace MAlice {

FunctionProcedureEntity::FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::list<unsigned short> parameterListTypes) : Entity(identifier, lineNumber)
{
    m_parameterListTypes = parameterListTypes;
}

}; // namespace MAlice