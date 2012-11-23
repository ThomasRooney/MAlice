
#ifndef _MALICEFUNCTIONPROCEDUREENTITY
#define _MALICEFUNCTIONPROCEDUREENTITY

#include <iostream>
#include <list>

#include "Entity.h"

namespace MAlice {
    
    class FunctionProcedureEntity : Entity {
    private:
        std::list<unsigned short> m_parameterListTypes;
        
    public:
        FunctionProcedureEntity(std::string identifier, std::list<unsigned short> parameterListTypes);
    };
    
};

#endif
