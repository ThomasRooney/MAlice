#include <string>
#include <fstream>
#include <sstream>

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include "CompilerContext.h"
#include "ErrorFactory.h"
#include "SyntacticAnalyser.h"
#include "SemanticAnalyser.h"
#include "Utilities.h"


#ifdef _WIN32
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned long int uint64_t;
#include <direct.h>
#include <io.h>
#define getcwd _getcwd
#define access _access

#else
#include <unistd.h>
#endif

#include "ErrorReporter.h"

using namespace MAlice;

std::string getPathFromCommandLineArguments(int argc, char *argv[]);

bool hasFlagsInCommandLineArguments(int argc, char *argv[]);
bool hasPrintTreeFlagEnabled(std::string flags);

int main(int argc, char *argv[])
{
    // Take the calling parameter into account
    argv++, argc--;
    
    bool printTree = false;
    
    if (hasFlagsInCommandLineArguments(argc, argv)) {
        std::string flags(argv[0]);
        
        printTree = hasPrintTreeFlagEnabled(flags);
        
        argv++, argc--;
    }
    
    ErrorReporter *errorReporter = new ErrorReporter();
    
    if (argc == 0) {
        errorReporter->reportError(ErrorFactory::createIOError("No input file specified"));
        
        // When we exit, memory used by the heap-allocated ErrorReporter will be freed by the system, so no need
        // to do it here.
        return EXIT_FAILURE;
    }

    std::string path = getPathFromCommandLineArguments(argc, argv);
    std::ifstream inputStream;
    inputStream.open(path);
    
    std::stringstream input;
    input << inputStream.rdbuf();
    
    CompilerContext *compilerContext = new CompilerContext(input.str());
    setParserErrorReporter(errorReporter);
    compilerContext->setErrorReporter(errorReporter);

    SyntacticAnalyser *syntacticAnalyser = new SyntacticAnalyser(compilerContext);
    ASTNode tree = NULL;
    
    if (syntacticAnalyser->parseInput(path, &tree)) {
        if (printTree)
            Utilities::printTree(tree);
        
        SemanticAnalyser *semanticAnalyser = new SemanticAnalyser(tree, compilerContext);
        semanticAnalyser->validate();
        
        delete semanticAnalyser;
    }
    
    delete syntacticAnalyser;
    delete errorReporter;
    delete compilerContext;

    return EXIT_SUCCESS;
}

bool hasFlagsInCommandLineArguments(int argc, char *argv[])
{
    if (argc == 0)
        return false;
    
    std::string firstArgument(argv[0]);
    if (!firstArgument.empty() && firstArgument[0] == '-')
        return true;
    
    return false;
}

bool hasPrintTreeFlagEnabled(std::string flags)
{
    for (std::string::iterator c = flags.begin();c != flags.end();++c) {
        if (*c == 't')
            return true;
    }
    
    return false;
}

std::string getPathFromCommandLineArguments(int argc, char *argv[])
{
    std::string path;
    // lets see if we can access file directly first
    if (access(argv[0], 0) == -1)
    {
        // File does not exist
        #ifdef _WIN32
        char *cwdpath = NULL;
        cwdpath = getcwd(cwdpath, NULL);
    
        if (cwdpath == 0)
            perror("getcwd error");
        path.append(cwdpath);
        path.append("\\");
        #endif
        if (argc < 1 || argv[0] == NULL)
            path.append("input");
        else
            path.append(argv[0]);

    }
    else{
      path = argv[0];
    }
    
    return path;
}