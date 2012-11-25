
#ifndef _MALICEFUNCTIONPROCEDUREENTITY
#define _MALICEFUNCTIONPROCEDUREENTITY

#include <iostream>
#include <list>

#include "Entity.h"
#include "Types.h"
#include "ParameterEntity.h"

namespace MAlice {
    
    class FunctionProcedureEntity : public Entity {
    private:
        std::list<ParameterEntity> m_parameterListTypes;
        
    public:
        FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::list<ParameterEntity> parameterListTypes);
        
        std::string humanReadableName() = 0;
        
        void setParameterListTypes(std::list<ParameterEntity> types);
    };
    
};

#endif
