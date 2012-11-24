
#ifndef _MALICEFUNCTIONPROCEDUREENTITY
#define _MALICEFUNCTIONPROCEDUREENTITY

#include <iostream>
#include <list>

#include "Entity.h"
#include "Types.h"

namespace MAlice {
    
    class FunctionProcedureEntity : public Entity {
    private:
        std::list<MAliceType> m_parameterListTypes;
        
    public:
        FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::list<MAliceType> parameterListTypes);
        
        std::string humanReadableName() = 0;
    };
    
};

#endif
