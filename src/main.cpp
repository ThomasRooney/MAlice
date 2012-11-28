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

typedef enum {
    CompilerFlagsNone = 0,
    CompilerFlagsPrintHelp,
    CompilerFlagsPrintAST
} CompilerFlags;

using namespace MAlice;
using namespace std;

std::string getPathFromCommandLineArguments(int argc, char *argv[]);

bool hasFlagsInCommandLineArguments(int argc, char *argv[]);
unsigned int getEnabledFlags(std::string flags);

void printHelp();

int main(int argc, char *argv[])
{
    // Take the calling parameter into account
    argv++, argc--;
    
    ErrorReporter *errorReporter = new ErrorReporter();
    
    unsigned int compilerFlags = CompilerFlagsNone;
    
    if (hasFlagsInCommandLineArguments(argc, argv)) {
        std::string flags(argv[0]);
        compilerFlags = getEnabledFlags(flags);
        
        if (compilerFlags == CompilerFlagsNone) {
            errorReporter->reportError(ErrorFactory::createIOError("Unknown flag(s) passed."));
            return EXIT_FAILURE;
        }
        
        argv++, argc--;
    }
    
    if (argc == 0 || (compilerFlags & CompilerFlagsPrintHelp) != 0) {
        printHelp();
        return EXIT_SUCCESS;
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
    
    if (!syntacticAnalyser->parseInput(path, &tree))
        return EXIT_FAILURE;
    
    if ((compilerFlags & CompilerFlagsPrintAST) != 0)
        Utilities::printTree(tree);
    
    SemanticAnalyser *semanticAnalyser = new SemanticAnalyser(tree, compilerContext);
    semanticAnalyser->validate();
    
    delete semanticAnalyser;
    
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

unsigned int getEnabledFlags(std::string flags)
{
    unsigned int retFlags = 0;
    
    for (std::string::iterator c = flags.begin();c != flags.end();++c) {
        if (*c == 't')
            retFlags |= CompilerFlagsPrintAST;
        if (*c == 'h')
            retFlags |= CompilerFlagsPrintHelp;
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

void printHelp()
{
    cout << "MAlice Compiler (by Thomas Rooney and Alex Rozanski)";
    cout << "\n\n" << "Usage: compile [Flags] [InputFile]";
    cout << "\n\nFlags:";
    cout << "\n" << "  -h    Print this help message";
    cout << "\n" << "  -t    Print parsed AST tree";
}