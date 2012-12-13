
#include "CodeGenerator.h"
#include <cstdio>
#include <sstream>
#include <fstream>

#include "Utilities.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/Support/ToolOutputFile.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif


namespace MAlice {
    
    CodeGenerator::CodeGenerator(llvm::Module *module)
    {
        m_module = module;
        m_dbinfo = NULL;
    }

    CodeGenerator::CodeGenerator(llvm::Module *module, llvm::DIBuilder *dbinfo)
    {
        m_module = module;
        m_dbinfo = dbinfo;
    }

    bool CodeGenerator::generateCode(std::string inputPath, std::string outputPath)
    {
        // Finalise the debug info
        if (m_dbinfo)
            m_dbinfo->finalize();
        
        std::string output;
        if (!m_module)
            return "";
        
        std::cout << "\n\nGenerating LLVM IR... ";
        
        // Create the function pass manager
        llvm::PassManager PM = llvm::PassManager();
/*        PM.add(llvm::createCFGSimplificationPass()); // Clean up code
        PM.add(llvm::createBasicAliasAnalysisPass()); 
        PM.add(llvm::createInstructionCombiningPass());
        PM.add(llvm::createPromoteMemoryToRegisterPass()); // Promote memory to registers where necessary
        PM.add(llvm::createScalarReplAggregatesPass()); // clean up allcas
        PM.add(llvm::createCFGSimplificationPass()); // Clean up again
        PM.add(llvm::createConstantPropagationPass()); // Constant Folding
        PM.add(llvm::createLICMPass()); // Loop Invariant optimization
        PM.add(llvm::createLoopDeletionPass()); // Dead loop deletion
        PM.add(llvm::createLoopUnrollPass()); // Unroll small loops
        PM.add(llvm::createSCCPPass()); // Constant propagation with different algorithm
        PM.add(llvm::createCFGSimplificationPass()); // Clean up..
        PM.add(llvm::createInstructionCombiningPass());
        PM.add(llvm::createDeadStoreEliminationPass()); // Delete dead stores
        PM.add(llvm::createAggressiveDCEPass()); // Delete dead instructions 
        PM.add(llvm::createCFGSimplificationPass());*/
        if (m_dbinfo) {
            PM.add(llvm::createDbgInfoPrinterPass()); // Create Debug Info
            PM.add(llvm::createModuleDebugInfoPrinterPass()); // Create Debug Info
        }

        std::string llvmIROutputPath = getLlvmIROutputPath(inputPath);
        std::string assemblyOutputPath = getAssemblyOutputPath(inputPath);

        llvm::raw_string_ostream outputStream(output);
        std::cout << "Writing IR to: " << llvmIROutputPath << std::endl;
        std::cout << "Writing ASM to: " << assemblyOutputPath << std::endl;

        std::string ErrorInfo;
        llvm::tool_output_file out(llvmIROutputPath.c_str(), ErrorInfo, llvm::raw_fd_ostream::F_Binary); 

        PM.add(llvm::createPrintModulePass(&out.os()));
        
        PM.run(*m_module);
        
	


        std::cout << "Done.";

        out.keep();

        std::ofstream llvmIROutputStream(llvmIROutputPath);
        llvmIROutputStream << output;
        
        // Run LLVM on the output
        std::cout << "\nRunning llc on LLVM IR... ";
        if (!runLlc(llvmIROutputPath, assemblyOutputPath)) {
            std::cerr << "\n\nFailed to create assembly with llc.";
            return false;
        }
        std::cout << " Done.";
        
        // Run clang to generate the executable
        std::cout << "\nRunning clang to generate executable... ";
        if (!runClang(assemblyOutputPath, outputPath)) {
            std::cerr << "\n\nFailed to create executable with clang.";
            return false;
        }
        std::cout << "Done.";
        
        std::cout << "\n\nExecutable generated at '" << outputPath << "'.";
        
        // Clean up temporary files.
        remove((char*)llvmIROutputPath.c_str());
        remove((char*)assemblyOutputPath.c_str());

        
        return true;
    }
    
    std::string CodeGenerator::getLlvmIROutputPath(const std::string inputPath)
    {
        return Utilities::getParentDirectoryForPath(inputPath) + DELIM + Utilities::getBaseFilenameFromPath(inputPath) + ".ll";
    }
    
    std::string CodeGenerator::getAssemblyOutputPath(const std::string inputPath)
    {
        return Utilities::getParentDirectoryForPath(inputPath) + DELIM + Utilities::getBaseFilenameFromPath(inputPath) + ".s";
    }
    
    bool CodeGenerator::runLlc(std::string inputPath, std::string outputPath)
    {
        std::string llcCall = "llc " + inputPath + " -o " + outputPath + " 2>&1";

        FILE *llcDescriptor = popen((char*)llcCall.c_str(), "r");
        if (pclose(llcDescriptor) != EXIT_SUCCESS)
            return false;
        
        return true;
    }
    
    bool CodeGenerator::runClang(std::string assemblyInputPath, std::string outputPath)
    {
        std::string clangCall = "clang " + assemblyInputPath + " -o " + outputPath + " 2>&1";
        
        FILE *clangDescriptor = popen((char*)clangCall.c_str(), "r");
        if (pclose(clangDescriptor) != EXIT_SUCCESS)
            return false;
        
        return true;
    }
    
}
