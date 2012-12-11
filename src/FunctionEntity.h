
#ifndef _MALICEFUNCTIONENTITY
#define _MALICEFUNCTIONENTITY

#include <iostream>
#include <list>

#include "FunctionProcedureEntity.h"
#include "Types.h"

namespace MAlice {

    class Type;
    
    class FunctionEntity : public FunctionProcedureEntity {
    private:
        Type m_returnType;
        
    public:
        FunctionEntity(std::string identifier, unsigned int lineNumber, std::vector<ParameterEntity*> parameterListTypes, Type returnType);
        FunctionEntity *clone();
        
        std::string humanReadableName();
        
        Type getReturnType() {return m_returnType;}
    }; // class FunctionEntity
    
}; // namespace MAlice

#endif /* ifndef _MALICEFUNCTIONENTITY */
