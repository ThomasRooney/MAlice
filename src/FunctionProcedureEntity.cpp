
#include "FunctionProcedureEntity.h"

namespace MAlice {

    FunctionProcedureEntity::FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::vector<ParameterEntity*> parameterListTypes) : Entity(identifier, lineNumber)
    {
        m_parameterListTypes = parameterListTypes;
        m_LLVMFunction = NULL;
        m_contextStructType = NULL;
    }
    
    void FunctionProcedureEntity::setParameterListTypes(std::vector<ParameterEntity*> types)
    {
        m_parameterListTypes = types;
    }
    
    std::vector<ParameterEntity*> FunctionProcedureEntity::getParameterListTypes()
    {
        return m_parameterListTypes;
    }
    
    bool FunctionProcedureEntity::getIsNestedFunction()
    {
        return m_isNestedFunction;
    }
    
    void FunctionProcedureEntity::setIsNestedFunction(bool isNested)
    {
        m_isNestedFunction = isNested;
    }

    llvm::Function *FunctionProcedureEntity::getLLVMFunction()
    {
        return m_LLVMFunction;
    }
    
    void FunctionProcedureEntity::setLLVMFunction(llvm::Function *function)
    {
        m_LLVMFunction = function;
    }
    
    llvm::StructType *FunctionProcedureEntity::getContextStructType()
    {
        return m_contextStructType;
    }
    
    void FunctionProcedureEntity::setContextStructType(llvm::StructType *contextStructType)
    {
        m_contextStructType = contextStructType;
    }
    
    std::vector<std::string> FunctionProcedureEntity::getCapturedVariables()
    {
        return m_capturedVariables;
    }
    
    void FunctionProcedureEntity::setCapturedVariables(std::vector<std::string> variables)
    {
        m_capturedVariables = variables;
    }
    
}; // namespace MAlice