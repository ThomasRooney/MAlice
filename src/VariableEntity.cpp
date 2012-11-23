
#include "VariableEntity.h"

namespace MAlice {

VariableEntity::VariableEntity(std::string identifier, unsigned short type) : Entity(identifier)
{
    m_type = type;
}

};