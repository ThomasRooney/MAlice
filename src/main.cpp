#include <string>

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include "SyntacticAnalyser.h"
#include "SemanticAnalyser.h"


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

void validateCompilerContext(CompilerContext *ctx);

// Skeleton file based on http://stackoverflow.com/a/8542203
int main(int argc, char *argv[])
{    
    std::string path = std::string();
    
#ifdef _WIN32
    char *cwdpath = NULL;
    cwdpath = getcwd(cwdpath, NULL);
    
    if (cwdpath == 0)
        perror("getcwd error");
    path.append(cwdpath);
    path.append("\\");
#endif
    
    if (argc < 2 || argv[1] == NULL)
        path.append("input");
    else
        path.append(argv[1]);
    
    ErrorReporter *errorReporter = new ErrorReporter();

    SyntacticAnalyser *syntacticAnalyser = new SyntacticAnalyser(path);
    syntacticAnalyser->setErrorReporter(errorReporter);
    ASTNode tree = syntacticAnalyser->parsedInput();
    
    SemanticAnalyser *semanticAnalyser = new SemanticAnalyser(tree);
    semanticAnalyser->setErrorReporter(errorReporter);
    semanticAnalyser->validate();
    
    delete semanticAnalyser;
    delete syntacticAnalyser;
    delete errorReporter;

    return EXIT_SUCCESS;
}