#ifndef _PARAMETERENTITY
#define _PARAMETERENTITY

#include "VariableEntity.h"

namespace MAlice {
    
    class Type;

    class ParameterEntity : public VariableEntity {
    private:
        bool m_byReference;
        
    public:
        ParameterEntity *clone();
        ParameterEntity (std::string identifier, int lineNumber, Type type);
        
        std::string humanReadableName();
        
        bool getByReference();
        void setByReference(bool byReference);
    }; // class ParameterEntity

}; // namespace MAlice

#endif //include guards