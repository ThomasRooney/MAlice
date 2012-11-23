
#ifndef _MALICEFUNCTIONENTITY
#define _MALICEFUNCTIONENTITY

#include <iostream>
#include <list>

#include "FunctionProcedureEntity.h"

namespace MAlice {

class FunctionEntity : FunctionProcedureEntity {
private:
    unsigned short m_returnType;
    
public:
    FunctionEntity(std::string identifier, std::list<unsigned short> parameterListTypes, unsigned short returnType);
    
}; // class FunctionEntity
    
}; // namespace MAlice

#endif /* ifndef _MALICEFUNCTIONENTITY */
