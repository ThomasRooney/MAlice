
#ifndef _MALICEPROCEDUREENTITY
#define _MALICEPROCEDUREENTITY

#include <iostream>
#include <list>

#include "FunctionProcedureEntity.h"

namespace MAlice {
  
    class ProcedureEntity : public FunctionProcedureEntity {
    public:
        ProcedureEntity(std::string identifier, std::list<unsigned short> parameterListTypes);
    };
    
};

#endif /* #ifndef _MALICEPROCEDUREENTITY */
