#ifndef _MALICEENTITY
#define _MALICEENTITY

#include <string>

namespace MAlice {
    
    class Entity {
    public:
      std::string m_identifier;
        
      Entity(std::string identifier);
        
    }; // class Entity
    
}; // namespace MAlice

#endif