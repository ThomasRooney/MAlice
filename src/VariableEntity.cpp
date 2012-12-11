
#include "VariableEntity.h"
#include "Type.h"

namespace MAlice {

    VariableEntity::VariableEntity(std::string identifier, unsigned int lineNumber, MAlice::Type type) : Entity(identifier, lineNumber), m_type(type)
    {
        m_LLVMValue = NULL;
    }
    
    std::string VariableEntity::humanReadableName()
    {
        return "variable";
    }

    Type VariableEntity :: getType() {
        return m_type;
    }
    
    llvm::Value *VariableEntity::getLLVMValue()
    {
        return m_LLVMValue;
    }
    
    void VariableEntity::setLLVMValue(llvm::Value *value)
    {
        m_LLVMValue = value;
    }

};