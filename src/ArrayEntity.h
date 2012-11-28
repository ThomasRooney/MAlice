
#include "VariableEntity.h"

namespace MAlice {

    class ArrayEntity : public VariableEntity {
    private:
        int length;
    public:
        ArrayEntity(std::string identifier, int lineNumber, MAliceType type, int length);
        
        std::string humanReadableName();
    }; // class ArrayEntity

}; // namespace MAlice