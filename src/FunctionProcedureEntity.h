
#ifndef _MALICEFUNCTIONPROCEDUREENTITY
#define _MALICEFUNCTIONPROCEDUREENTITY

#include <iostream>
#include <list>

#include "Entity.h"
#include "Types.h"
#include "ParameterEntity.h"

namespace MAlice {
    
    class FunctionProcedureEntity : public Entity {
    private:
        std::vector<ParameterEntity*> m_parameterListTypes;
        bool m_isNestedFunction;
        llvm::Function *m_LLVMFunction;
        llvm::StructType *m_contextStructType;
        std::vector<std::string> m_capturedVariables;
        
    public:
        FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::vector<ParameterEntity*> parameterListTypes);
        virtual FunctionProcedureEntity *clone() = 0;
        
        virtual std::string humanReadableName() = 0;
        
        void setParameterListTypes(std::vector<ParameterEntity*> types);
        std::vector<ParameterEntity*> getParameterListTypes();
        
        bool getIsNestedFunction();
        void setIsNestedFunction(bool isNested);
    
        llvm::Function *getLLVMFunction();
        void setLLVMFunction(llvm::Function *function);
        
        llvm::StructType *getContextStructType();
        void setContextStructType(llvm::StructType *contextStructType);
        
        std::vector<std::string> getCapturedVariables();
        void setCapturedVariables(std::vector<std::string> variables);
    };
    
};

#endif
