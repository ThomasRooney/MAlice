#ifndef _MALICEENTITY
#define _MALICEENTITY

#include <string>

namespace MAlice {
    
class Entity {
public:
  std::string name;
  unsigned short type;

  Entity(std::string name,unsigned short type);
    
}; // class Entity
    
}; // namespace MAlice

#endif