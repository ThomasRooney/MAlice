
#ifndef _MALICECODEGENERATOR
#define _MALICECODEGENERATOR

#include <iostream>

#include "LLVMHeader.h"

namespace MAlice {
    
    class CodeGenerator {
    private:
        llvm::Module *m_module;
        llvm::DIBuilder *m_dbinfo;
        void cleanUp(std::string outputPath);
        std::string llvmIROutputPath(std::string path);
        std::string assemblyOutputPath(std::string path);
    public:
        CodeGenerator(llvm::Module *module);
        CodeGenerator(llvm::Module *module, llvm::DIBuilder *dbinfo);
        
        bool generateCode(std::string outputPath);
    };
    
}

#endif // #ifndef _MALICECODEGENERATOR
