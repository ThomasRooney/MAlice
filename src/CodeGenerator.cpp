
#include "CodeGenerator.h"
#include <cstdio>
#include <sstream>
#include <fstream>
#include "Utilities.h"

#ifdef _WIN32
#undef min
#undef max
#endif

#include "llvm/Support/raw_ostream.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/FormattedStream.h"

#include "llvm/ADT/Triple.h"
#include "llvm/Support/Host.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"

#include "signal.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif




namespace MAlice {
    

    

    CodeGenerator::CodeGenerator(llvm::Module *module)
    {
        m_module = module;
        m_dbinfo = NULL;
        CodeGenerator::instance = this;
    }

    CodeGenerator::CodeGenerator(llvm::Module *module, llvm::DIBuilder *dbinfo)
    {
        m_module = module;
        m_dbinfo = dbinfo;
        CodeGenerator::instance = this;
    }

    CodeGenerator * CodeGenerator::instance = 0;

    void CodeGenerator::clean_exit_handler(int sign_num)
    {
        std::cerr << "Fatal Error in Code Generation. " << (m_dbinfo?"":" Try running with the -g option to get more information.") << std::endl;
        if (m_dbinfo)
        {
            std::cerr << "Attempting to output a dump of the .ll file to stdout for debugging....." << std::endl;
            m_module->dump();
            std::cout << std::endl;
        }
        exit(1);
    }

    bool CodeGenerator::generateCode(std::string inputPath, std::string outputPath, bool optimisationsOn)
    {
        // Finalise the debug info
        if (m_dbinfo)
            m_dbinfo->finalize();
        
        std::string output;
        if (!m_module)
            return "";
        
        std::cout << "\n\nGenerating LLVM IR... " << std::endl;
        
        // Create the function pass manager
        llvm::PassManager PM = llvm::PassManager();
        if (optimisationsOn) {
            PM.add(llvm::createCFGSimplificationPass()); // Clean up code
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
            PM.add(llvm::createCFGSimplificationPass());
        }

        std::string llvmIROutputPath = getLlvmIROutputPath(inputPath);
        std::string assemblyOutputPath = getAssemblyOutputPath(inputPath);

        llvm::raw_string_ostream outputStream(output);
        std::cout << "    Writing IR to: " << llvmIROutputPath << std::endl;
        std::cout << "    Writing ASM to: " << assemblyOutputPath << std::endl;

        std::string ErrorInfo;
        llvm::tool_output_file out(llvmIROutputPath.c_str(), ErrorInfo, llvm::raw_fd_ostream::F_Binary); 

        PM.add(llvm::createPrintModulePass(&out.os()));

        // Generate bitcode for the current target

        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmPrinters();
        llvm::InitializeAllAsmParsers();

        llvm::Triple TheTriple(m_module->getTargetTriple());
        TheTriple.setTriple(llvm::sys::getHostTriple());
        std::string Err;
        const llvm::Target *TheTarget = llvm::TargetRegistry::lookupTarget(TheTriple.getTriple(), Err);

        if (TheTarget == 0) {
            std::cerr << "Fatal Error generating bitcode: " << Err << std::endl;
            PM.run(*m_module);
            return false;
        }

        std::string AsmErrorInfo;
        llvm::tool_output_file
          outAsm(assemblyOutputPath.c_str(), AsmErrorInfo, llvm::raw_fd_ostream::F_Binary); 

          std::auto_ptr<llvm::TargetMachine>
            target(TheTarget->createTargetMachine(TheTriple.getTriple(),
                                                  "", "",
                                                  llvm::Reloc::Default, llvm::CodeModel::Default));

        llvm::TargetMachine &Target = *target.get();

        // Create target
        if (const llvm::TargetData *TD = Target.getTargetData())
            PM.add(new llvm::TargetData(*TD));
        else
            PM.add(new llvm::TargetData(m_module));
        
        Target.setAsmVerbosityDefault(true);
        
        llvm::CodeGenOpt::Level optLevel = optimisationsOn?llvm::CodeGenOpt::Aggressive:llvm::CodeGenOpt::None;

        llvm::formatted_raw_ostream ASOS(outAsm.os());
        if (Target.addPassesToEmitFile(PM, ASOS, llvm::TargetMachine::CGFT_AssemblyFile, optLevel)){
            std::cerr << "Fatal Error, cannot add passes to generate " << assemblyOutputPath << std::endl;
            return false;
        }
        try {
            signal(SIGINT, CodeGenerator::clean_exit_on_sig);
            signal(SIGABRT, CodeGenerator::clean_exit_on_sig);
            signal(SIGILL, CodeGenerator::clean_exit_on_sig);
            signal(SIGFPE, CodeGenerator::clean_exit_on_sig);
            signal(SIGSEGV, CodeGenerator::clean_exit_on_sig);
            signal(SIGTERM, CodeGenerator::clean_exit_on_sig);

            PM.run(*m_module);
        } catch (...) {
            std::cerr << "Fatal Error in Code Generation. " << (m_dbinfo?"":" Try running with the -g option to get more information.") << std::endl;
            if (m_dbinfo)
            {
                std::cerr << "Attempting to output a dump of the .ll file to stdout for debugging....." << std::endl;
                m_module->dump();
                std::cout << std::endl;
            }
            return false;
        }
        std::cout << ".....Done." << std::endl;

        out.keep();
        outAsm.keep();
        // Flush the file so that clang can access it.
        ASOS.flush();

        // Run LLVM on the output
        // std::cout << "\nRunning llc on LLVM IR... ";
        // if (!runLlc(llvmIROutputPath, assemblyOutputPath)) {
        //     std::cerr << "Failed to create assembly with llc." << std::endl;
        //     return false;
        // }
        // std::cout << " Done.";
        
        // Run clang to generate the executable
        std::cout << "\nRunning clang to generate executable... ";
        if (!runClang(assemblyOutputPath, outputPath)) {
            std::cerr << "\nFailed to create executable with clang." << std::endl;
            return false;
        }
        std::cout << "Done.";
        
        std::cout << "\n\nExecutable generated at '" << outputPath << "'." << std::endl;
        
        // Clean up temporary files.
//        remove((char*)llvmIROutputPath.c_str());
//        remove((char*)assemblyOutputPath.c_str());

        
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
        std::string llcCall = "dependencies/llvm/bin/llc " + inputPath + " -o " + outputPath + " 2>&1";

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

