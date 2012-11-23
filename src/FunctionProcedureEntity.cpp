
#include "FunctionProcedureEntity.h"

namespace MAlice {

FunctionProcedureEntity::FunctionProcedureEntity(std::string identifier, std::list<unsigned short> parameterListTypes) : Entity(identifier)
{
    m_parameterListTypes = parameterListTypes;
}

}; // namespace MAlice