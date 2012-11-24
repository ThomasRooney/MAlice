
#include "Entity.h"

using namespace std;

namespace MAlice {
    
    Entity::Entity(string identifier, unsigned int lineNumber)
    {
        m_identifier = identifier;
        m_lineNumber = lineNumber;
    }
    
    Entity::~Entity()
    {
    }
    
    std::string Entity::getIdentifier()
    {
        return m_identifier;
    }
    
    unsigned int Entity::getLineNumber()
    {
        return m_lineNumber;
    }
    
}; // namespace MAlice