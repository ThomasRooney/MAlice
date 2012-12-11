#ifndef _OPTIMIZER
#define _OPTIMIZER
#include "LLVMHeader.h"
#include "Types.h"
#include "CompilerContext.h"

namespace MAlice {
    
    class Optimizer  {
    private:
        llvm::Module *m_module;
    public:                
        Optimizer (llvm::Module*);
        void constantFoldingPass(ASTNode node, CompilerContext* ctx);
        llvm::Module *getModule();
    };
    
    
}

#endif