#ifndef _PARAMETERENTITY
#define _PARAMETERENTITY

#include "VariableEntity.h"

namespace MAlice {

class ParameterEntity : public VariableEntity {
private:
  bool passedByReference;
public:
  bool isPassedByReference () { return passedByReference; }
  bool isPassedByValue() { return !passedByReference; }
  ParameterEntity *clone();
  ParameterEntity (std::string identifier, int lineNumber, MAliceType type, bool passedByReference);
}; // class ParameterEntity

}; // namespace MAlice

#endif //include guards