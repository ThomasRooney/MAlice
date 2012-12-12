
#include "CodeGenerator.h"
#include <cstdio>
#include <sstream>
#include <fstream>

#include "Utilities.h"
#include "llvm/Support/raw_ostream.h"

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
        
        // Print the LLVM output into a string
        llvm::raw_string_ostream outputStream(output);
        m_module->print(outputStream, NULL);
        
        std::cout << output;
        
        std::cout << "Done.";

        std::string llvmIROutputPath = getLlvmIROutputPath(inputPath);
        std::string assemblyOutputPath = getAssemblyOutputPath(inputPath);
        
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
    
    std::string CodeGenerator::getLlvmIROutputPath(std::string inputPath)
    {
        return Utilities::getParentDirectoryForPath(inputPath) + "/" + Utilities::getBaseFilenameFromPath(inputPath) + ".ll";
    }
    
    std::string CodeGenerator::getAssemblyOutputPath(std::string inputPath)
    {
        return Utilities::getParentDirectoryForPath(inputPath) + "/" + Utilities::getBaseFilenameFromPath(inputPath) + ".s";
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