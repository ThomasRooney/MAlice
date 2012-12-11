
#ifndef _MALICEVARIABLEENTITY
#define _MALICEVARIABLEENTITY

#include <iostream>

#include "Entity.h"
#include "Types.h"
#include "Type.h"

#include "llvm/Value.h"

namespace MAlice {
    
    class Type;

    class VariableEntity : public Entity {
    private:
        Type m_type;
        llvm::Value *m_LLVMValue;
    public:
        VariableEntity(std::string identifier, unsigned int lineNumber, Type type);
        
        std::string humanReadableName();
        Type getType();
        
        llvm::Value *getLLVMValue();
        void setLLVMValue(llvm::Value *value);
        
    }; // class VariableEntity
    
}; // namespace MAlice

#endif
