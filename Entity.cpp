#include "Entity.h"

using namespace std;

namespace MAlice {
    
    Entity::Entity(string identifier, EntityType type)
    {
        m_identifier = identifier;
        m_type = type;
    }
    
}; // namespace MAlice