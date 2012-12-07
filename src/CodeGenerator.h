
#ifndef _MALICECODEGENERATOR
#define _MALICECODEGENERATOR

#include <iostream>

#include "llvm/Module.h"

namespace MAlice {
    
    class CodeGenerator {
    private:
        llvm::Module *m_module;
    public:
        CodeGenerator(llvm::Module *module);
        
        std::string generateCode();
    };
    
}

#endif // #ifndef _MALICECODEGENERATOR
