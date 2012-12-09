
#include "VariableEntity.h"

namespace MAlice {

    VariableEntity::VariableEntity(std::string identifier, unsigned int lineNumber, MAliceType type) : Entity(identifier, lineNumber)
    {
        m_type = type;
        m_LLVMValue = NULL;
    }
    
    VariableEntity::~VariableEntity()
    {
    }
    
    std::string VariableEntity::humanReadableName()
    {
        return "variable";
    }

    MAliceType VariableEntity :: getType() {
        return this->m_type;
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