
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
        llvm::Function *m_LLVMFunction;
        llvm::StructType *m_contextStructType;
        
    public:
        FunctionProcedureEntity(std::string identifier, unsigned int lineNumber, std::vector<ParameterEntity*> parameterListTypes);
        virtual FunctionProcedureEntity *clone() = 0;
        
        virtual std::string humanReadableName() = 0;
        
        void setParameterListTypes(std::vector<ParameterEntity*> types);
        std::vector<ParameterEntity*> getParameterListTypes();
    
        llvm::Function *getLLVMFunction();
        void setLLVMFunction(llvm::Function *function);
        
        llvm::StructType *getContextStructType();
        void setContextStructType(llvm::StructType *contextStructType);
    };
    
};

#endif
