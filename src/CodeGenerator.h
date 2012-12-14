
#ifndef _MALICECODEGENERATOR
#define _MALICECODEGENERATOR

#include <iostream>

#include "LLVMHeader.h"

namespace MAlice {
    
    class CodeGenerator {
    private:
        llvm::Module *m_module;
        llvm::DIBuilder *m_dbinfo;
        std::string getLlvmIROutputPath(std::string inputPath);
        std::string getAssemblyOutputPath(std::string inputPath);
        
        bool runLlc(std::string inputPath, std::string outputPath);
        bool runClang(std::string assemblyInputPath, std::string outputPath);
    public:
        CodeGenerator(llvm::Module *module);
        CodeGenerator(llvm::Module *module, llvm::DIBuilder *dbinfo);
        
        bool generateCode(std::string inputPath, std::string outputPath, bool optimisationsOn);
    };
    
}

#endif // #ifndef _MALICECODEGENERATOR
