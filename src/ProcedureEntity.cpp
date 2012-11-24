
#include "ProcedureEntity.h"

namespace MAlice {

ProcedureEntity::ProcedureEntity(std::string identifier, unsigned int lineNumber, std::list<unsigned short> parameterListTypes) : FunctionProcedureEntity(identifier, lineNumber, parameterListTypes)
{
}

}; // namespace MAlice