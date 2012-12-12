
#include "CodeGenerator.h"

#include <cstdio>
#include <sstream>
#include <fstream>
#include "Utilities.h"
#include "llvm/Support/raw_ostream.h"

namespace MAlice {
    
    CodeGenerator::CodeGenerator(llvm::Module *module)
    {
        m_module = module;
    }

    CodeGenerator::CodeGenerator(llvm::Module *module, llvm::DIBuilder *dbinfo)
    {
        m_dbinfo = dbinfo;
        CodeGenerator::CodeGenerator(module);
    }

    bool CodeGenerator::generateCode(std::string outputPath)
    {
        // Finalise the debug info
        m_dbinfo->finalize();

        std::string output;
        if (!m_module)
            return "";
        
        // Print the LLVM output into a string
        llvm::raw_string_ostream outputStream(output);
        m_module->print(outputStream, NULL);

        std::cout << output;
        
        std::string llvmIROutputFile = llvmIROutputPath(outputPath);
        std::string assemblyOutputFile = assemblyOutputPath(outputPath);
        
        std::ofstream llvmIROutputStream(llvmIROutputFile);
        llvmIROutputStream << output;
        
        // Run LLVM on the output
        std::string llcCall = "/opt/local/bin/llc-mp-3.0 " + llvmIROutputFile;
        std::cerr << llcCall << std::endl;
            
        system((char*)llcCall.c_str());
        
        std::string clangCall = "clang -v " + assemblyOutputFile;
        std::cerr << clangCall << std::endl;
        system((char*)clangCall.c_str());
        
        // Clean up temporary files.
        cleanUp(outputPath);
        
        return true;
    }
    
    void CodeGenerator::cleanUp(std::string outputPath)
    {
        std::string llvmPath = llvmIROutputPath(outputPath);
        remove((char*)llvmPath.c_str());
    }
    
    std::string CodeGenerator::llvmIROutputPath(std::string path)
    {
        return Utilities::getParentDirectoryForPath(path) + "/" + Utilities::getBaseFilenameFromPath(path) + ".ll";
    }
    
    std::string CodeGenerator::assemblyOutputPath(std::string path)
    {
        return Utilities::getParentDirectoryForPath(path) + "/" + Utilities::getBaseFilenameFromPath(path) + ".s";
    }
    
}