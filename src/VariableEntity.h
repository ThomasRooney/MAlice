
#ifndef _MALICEVARIABLEENTITY
#define _MALICEVARIABLEENTITY

#include <iostream>

#include "Entity.h"
#include "Types.h"

#include "llvm/Value.h"

namespace MAlice {

class VariableEntity : public Entity {

private:
    MAliceType m_type;
    llvm::Value *m_LLVMValue;
public:
    VariableEntity(std::string identifier, unsigned int lineNumber, MAliceType type);
    ~VariableEntity();
    
    std::string humanReadableName();
    MAliceType getType();
    
    llvm::Value *getLLVMValue();
    void setLLVMValue(llvm::Value *value);
    
}; // class VariableEntity
    
}; // namespace MAlice

#endif
