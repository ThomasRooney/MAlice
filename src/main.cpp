#include <string>

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include "SyntacticAnalyser.h"
#include "SemanticAnalyser.h"
#include "Utilities.h"


#ifdef _WIN32
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned long int uint64_t;
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

using namespace MAlice;

std::string getPathFromCommandLineArguments(int argc, char *argv[]);

bool hasFlagsInCommandLineArguments(int argc, char *argv[]);
bool hasPrintTreeFlagEnabled(std::string flags);

// Skeleton file based on http://stackoverflow.com/a/8542203
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
    
    std::string path = getPathFromCommandLineArguments(argc, argv);
    
    ErrorReporter *errorReporter = new ErrorReporter();

    SyntacticAnalyser *syntacticAnalyser = new SyntacticAnalyser(path);
    syntacticAnalyser->setErrorReporter(errorReporter);
    ASTNode tree = syntacticAnalyser->parsedInput();
    
    if (printTree)
        Utilities::printTree(tree);
    
    SemanticAnalyser *semanticAnalyser = new SemanticAnalyser(tree);
    semanticAnalyser->setErrorReporter(errorReporter);
    semanticAnalyser->validate();
    
    delete semanticAnalyser;
    delete syntacticAnalyser;
    delete errorReporter;

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
    for (char& c : flags) {
        if (c == 't')
            return true;
    }
    
    return false;
}

std::string getPathFromCommandLineArguments(int argc, char *argv[])
{
    std::string path;
    
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
    
    return path;
}