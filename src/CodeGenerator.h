
#ifndef _MALICECODEGENERATOR
#define _MALICECODEGENERATOR

#include <iostream>

#include "llvm/Module.h"

namespace MAlice {
    
    class CodeGenerator {
    private:
        llvm::Module *m_module;
        
        void cleanUp(std::string outputPath);
        std::string llvmIROutputPath(std::string path);
        std::string assemblyOutputPath(std::string path);
    public:
        CodeGenerator(llvm::Module *module);
        
        bool generateCode(std::string outputPath);
    };
    
}

#endif // #ifndef _MALICECODEGENERATOR
