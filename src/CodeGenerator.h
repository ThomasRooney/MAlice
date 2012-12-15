
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
        
        // Handle segfaults in the LLVM libraries correctly.
        static CodeGenerator *instance; 
        static void clean_exit_on_sig(int sig_num) // Limitation of gcc means
        {                                          // static functions must be 
            instance->clean_exit_handler(sig_num); // inline in the function 
        }                                          // definition
        void clean_exit_handler(int sig_num);
        
        bool runLlc(std::string inputPath, std::string outputPath);
        bool runClang(std::string assemblyInputPath, std::string outputPath);
    public:

        CodeGenerator(llvm::Module *module);
        CodeGenerator(llvm::Module *module, llvm::DIBuilder *dbinfo);
        
        bool generateCode(std::string inputPath, std::string outputPath, bool optimisationsOn);
    };
    
}

#endif // #ifndef _MALICECODEGENERATOR
