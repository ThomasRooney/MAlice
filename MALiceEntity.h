#ifndef _MALICEENTITY
#define _MALICEENTITY

#include <string>

class MAliceEntity {
public:
  std::string name;
  unsigned short type;

  MAliceEntity(std::string name,unsigned short type);

};

#endif