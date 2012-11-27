
#ifndef _MALICEFUNCTIONENTITY
#define _MALICEFUNCTIONENTITY

#include <iostream>
#include <list>

#include "FunctionProcedureEntity.h"
#include "Types.h"

namespace MAlice {

class FunctionEntity : public FunctionProcedureEntity {
private:
    MAliceType m_returnType;
    
public:
    FunctionEntity(std::string identifier, unsigned int lineNumber, std::list<ParameterEntity> parameterListTypes, MAliceType returnType);
    FunctionEntity *clone();
    
    std::string humanReadableName();
    
    MAliceType getReturnType() {return m_returnType;}
}; // class FunctionEntity
    
}; // namespace MAlice

#endif /* ifndef _MALICEFUNCTIONENTITY */
