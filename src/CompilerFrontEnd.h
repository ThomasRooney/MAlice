#ifndef _MALICECOMPILERFRONTEND
#define _MALICECOMPILERFRONTEND

#include <iostream>
#include "llvm/Module.h"

namespace MAlice {
    
    class ErrorReporter;
    
    class CompilerFrontEnd {
    private:
        int m_argc;
        char **m_argv;
        ErrorReporter *m_errorReporter;
        
        bool hasFlagsInCommandLineArguments();
        unsigned int getEnabledFlags();
        void printHelp();
        std::string getPathFromCommandLineArguments();
        void printErrorReport();
        
        std::string llvmOutputPath(std::string path);
    public:
        CompilerFrontEnd(int argc, char **argv);
        ~CompilerFrontEnd();
        
        bool run();
    }; // namespace CompilerFrontEnd
    
} // namespace MAlice

#endif // #ifndef _MALICECOMPILERFRONTEND
