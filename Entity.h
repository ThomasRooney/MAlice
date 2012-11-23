#ifndef _MALICEENTITY
#define _MALICEENTITY

#include <string>

namespace MAlice {
    
    typedef enum {
        EntityTypeUndefined = 0,
        EntityTypeVariable,
        EntityTypeProcedure,
        EntityTypeFunction
    } EntityType;
    
    class Entity {
    public:
      std::string m_identifier;
      EntityType m_type;

      Entity(std::string identifier, EntityType type);
        
    }; // class Entity
    
}; // namespace MAlice

#endif