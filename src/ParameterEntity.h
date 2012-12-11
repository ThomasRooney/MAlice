#ifndef _PARAMETERENTITY
#define _PARAMETERENTITY

#include "VariableEntity.h"

namespace MAlice {
    
    class Type;

    class ParameterEntity : public VariableEntity {
    public:
        ParameterEntity *clone();
        ParameterEntity (std::string identifier, int lineNumber, Type type);
        
        std::string humanReadableName();
    }; // class ParameterEntity

}; // namespace MAlice

#endif //include guards