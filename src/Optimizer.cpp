#include "Optimizer.h"

namespace MAlice {


        Optimizer::Optimizer (llvm::Module* module)
        {
            m_module = module;
        }

        void Optimizer::constantFoldingPass()
        {

        }
    
        llvm::Module *Optimizer::getModule()
        {
            return m_module;
        }
}