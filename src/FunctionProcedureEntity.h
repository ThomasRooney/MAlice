
#ifndef _MALICEFUNCTIONPROCEDUREENTITY
#define _MALICEFUNCTIONPROCEDUREENTITY

#include <iostream>
#include <list>

#include "Entity.h"

namespace MAlice {
    
    class FunctionProcedureEntity : public Entity {
    private:
        std::list<unsigned short> m_parameterListTypes;
        
    public:
        FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::list<unsigned short> parameterListTypes);
        
        std::string humanReadableName() = 0;
    };
    
};

#endif
