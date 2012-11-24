
#ifndef _MALICEPROCEDUREENTITY
#define _MALICEPROCEDUREENTITY

#include <iostream>
#include <list>

#include "FunctionProcedureEntity.h"
#include "Types.h"

namespace MAlice {
  
    class ProcedureEntity : public FunctionProcedureEntity {
    public:
        ProcedureEntity(std::string identifier, unsigned int lineNumber, std::list<MAliceType> parameterListTypes);
        
        std::string humanReadableName();
    };
    
};

#endif /* #ifndef _MALICEPROCEDUREENTITY */
