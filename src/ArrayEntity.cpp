#include "ArrayEntity.h"

namespace MAlice {
    ArrayEntity::ArrayEntity(std::string identifier, int lineNumber, MAliceType type, int length) : VariableEntity(identifier, lineNumber, type)
    {
        this->length = length;
    }
    
    std::string ArrayEntity::humanReadableName()
    {
        return "array";
    }

};