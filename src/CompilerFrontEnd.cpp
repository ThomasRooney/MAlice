
#include <fstream>
#include <sstream>
#include <cstdio>

#include "CompilerFrontEnd.h"
#include "Optimizer.h"
#include "CompilerContext.h"
#include "ErrorFactory.h"
#include "ErrorReporter.h"
#include "SyntacticAnalyser.h"
#include "CodeGenerator.h"
#include "SemanticAnalyser.h"
#include "Utilities.h"

#include "llvm/Support/raw_os_ostream.h"

#ifdef _WIN32
#ifndef _LLVM_HEADER_
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned long int uint64_t;
#endif
#include <direct.h>
#include <io.h>
#define getcwd _getcwd
#define access _access
#else
#include <unistd.h>
#include <libgen.h>
#endif

using namespace std;

namespace MAlice {
    
    typedef enum {
        CompilerFlagsNone = 0,
        CompilerFlagsPrintHelp = 1,
        CompilerFlagsPrintAST = 2,
        CompilerFlagsDebugInformation = 4,
    } CompilerFlags;
  
    CompilerFrontEnd::CompilerFrontEnd(int argc, char **argv)
    {
        // Take the calling parameter into account
        argv++, argc--;
        
        m_argc = argc;
        m_argv = argv;
        
        m_errorReporter = new ErrorReporter();
    }
    
    CompilerFrontEnd::~CompilerFrontEnd()
    {
        if (m_errorReporter) {
            delete m_errorReporter;
            m_errorReporter = NULL;
        }
    }
    
    bool CompilerFrontEnd::run()
    {
        unsigned int compilerFlags = CompilerFlagsNone;
        
        while (hasFlagsInCommandLineArguments()) {
            compilerFlags = getEnabledFlags();
            
            if (compilerFlags == CompilerFlagsNone) {
                m_errorReporter->reportError(ErrorFactory::createIOError("Unknown flag(s) passed."));
                return EXIT_FAILURE;
            }
            
            m_argv++, m_argc--;
        }
        
        if (m_argc == 0 || (compilerFlags & CompilerFlagsPrintHelp) != 0) {
            printHelp();
            return EXIT_SUCCESS;
        }
        
        std::string path = getPathFromCommandLineArguments();
        std::ifstream inputStream;
        inputStream.open(path);
        
        std::stringstream input;
        input << inputStream.rdbuf();
        CompilerContext *compilerContext;
        if ((compilerFlags & CompilerFlagsDebugInformation) != 0)
        {
            compilerContext = new CompilerContext(input.str(), Utilities::getBaseFilenameFromPath(path), Utilities::getParentDirectoryForPath(path));
        }
        else{
            compilerContext = new CompilerContext(input.str());
        }
        setParserErrorReporter(m_errorReporter);
        compilerContext->setErrorReporter(m_errorReporter); 
        
        cout << "Analysing file '" + path + "'...\n\n";
        
        SyntacticAnalyser *syntacticAnalyser = new SyntacticAnalyser(compilerContext);
        ASTNode tree = NULL;
        
        SemanticAnalyser *semanticAnalyser = NULL;
        
        if (!syntacticAnalyser->parseInput(path, &tree)) {
            printErrorReport();
            return EXIT_FAILURE;
        }
    
        if ((compilerFlags & CompilerFlagsPrintAST) != 0)
            Utilities::printTree(tree);
        
        semanticAnalyser = new SemanticAnalyser(tree, compilerContext);
        if (!semanticAnalyser->validateAST()) {
            // Exit before generation if there are serious errors
            return EXIT_FAILURE;
        }
        // Reset Compiler Context after validation, TODO: Generate once.
        compilerContext->clearSemanticInformation();
        llvm::Module *module = NULL;

        
        if (!semanticAnalyser->generateIR(&module)) {
            std::cerr << "Failure";
            return EXIT_FAILURE;
        }
        
        // Do optimisation and output code
        
        Optimizer optimizer();

        //optimizationPass.constantFoldingPass();
        
        std::string outputPath = Utilities::getParentDirectoryForPath(path) + "/" + Utilities::getBaseFilenameFromPath(path);
        
        CodeGenerator generator(module);
        generator.generateCode(path, outputPath);

        if (semanticAnalyser) {
            delete semanticAnalyser;
            semanticAnalyser = NULL;
        }
        
        delete syntacticAnalyser;
        delete compilerContext;
        
        return EXIT_SUCCESS;
    }
    
    bool CompilerFrontEnd::hasFlagsInCommandLineArguments()
    {
        if (m_argc == 0)
            return false;
        
        std::string firstArgument(m_argv[0]);
        if (!firstArgument.empty() && firstArgument[0] == '-')
            return true;
        
        return false;
    }
    
    unsigned int CompilerFrontEnd::getEnabledFlags()
    {
        if (m_argc == 0)
            return CompilerFlagsNone;
        
        std::string flags(m_argv[0]);
        unsigned int retFlags = 0;
        
        for (std::string::iterator c = flags.begin();c != flags.end();++c) {
            if (*c == 't')
                retFlags |= CompilerFlagsPrintAST;
            if (*c == 'h')
                retFlags |= CompilerFlagsPrintHelp;
            if (*c == 'd')
                retFlags |= CompilerFlagsDebugInformation;

        }
        
        return retFlags;
    }
    
    std::string CompilerFrontEnd::getPathFromCommandLineArguments()
    {
        std::string path;
        // lets see if we can access file directly first
        if (access(m_argv[0], 0) == -1)
        {
            // File does not exist
#ifdef _WIN32
            char *cwdpath = NULL;
            cwdpath = getcwd(cwdpath, NULL);
            
            if (cwdpath == 0)
                m_errorReporter->reportError(ErrorFactory::createIOError("getcwd error"));
            path.append(cwdpath);
            path.append("\\");
#endif
            if (m_argc < 1 || m_argv[0] == NULL)
                path.append("input");
            else
                path.append(m_argv[0]);
            
        }
        else{
            path = m_argv[0];
        }
        
        return path;
    }
    
    void CompilerFrontEnd::printHelp()
    {
        cout << "MAlice Compiler (by Thomas Rooney and Alex Rozanski)";
        cout << "\n\n" << "Usage: compile [Flags] [InputFile]";
        cout << "\n\nFlags:";
        cout << "\n" << "  -h    Print this help message";
        cout << "\n" << "  -t    Print parsed AST tree";
        cout << endl;
    }
    
    void CompilerFrontEnd::printErrorReport()
    {
        if (!m_errorReporter->hasReportedErrors() && !m_errorReporter->hasReportedWarnings())
            cout << "No errors found.\n";
        else {
            if (m_errorReporter->hasReportedWarnings()) {
                unsigned int numberOfWarnings = m_errorReporter->getNumberOfReportedWarnings();
                cout << Utilities::numberToString(numberOfWarnings) + " " +
                ((numberOfWarnings == 1) ? "warning" : "warnings") +
                ".\n";
            }
            
            if (m_errorReporter->hasReportedErrors()) {
                unsigned int numberOfErrors = m_errorReporter->getNumberOfReportedErrors();
                cout << Utilities::numberToString(numberOfErrors) + " " +
                ((numberOfErrors == 1) ? "error" : "errors") +
                " found.\n";
            }
        }
    }
    
    std::string CompilerFrontEnd::llvmOutputPath(std::string path)
    {
        std::string parent = Utilities::getParentDirectoryForPath(path);
        std::string baseFilename = Utilities::getBaseFilenameFromPath(path);
        
        return parent + "/" + baseFilename + ".ll";
    }
    
};