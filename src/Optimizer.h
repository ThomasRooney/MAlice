#ifndef _OPTIMIZER
#define _OPTIMIZER
#include "LLVMHeader.h"

namespace MAlice {
    
    class Optimizer  {
    private:
        llvm::Module *m_module;
    public: 
        Optimizer (llvm::Module*);
        void constantFoldingPass();
        llvm::Module *getModule();
    };
    
    
}

#endif