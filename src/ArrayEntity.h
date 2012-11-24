
#include "VariableEntity.h"

namespace MAlice {

class ArrayEntity : public VariableEntity {
public:
    ArrayEntity(std::string identifier, int lineNumber, MAliceType type);
};

};