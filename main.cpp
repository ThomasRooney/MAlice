#include <string>

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include "ASTWalker.h"


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

// Skeleton file based on http://stackoverflow.com/a/8542203
int main(int argc, char *argv[])
{    
    ASTWalker treeWalker = ASTWalker();

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
    
    pANTLR3_UINT8 filePath;
    pANTLR3_INPUT_STREAM input;
    
    filePath = (pANTLR3_UINT8)path.c_str();
    input = antlr3FileStreamNew(filePath, ANTLR3_ENC_UTF8);

    if (!input)
        return EXIT_FAILURE;

    pMAliceLexer lexer = MAliceLexerNew(input);
    if (!lexer)
        return EXIT_FAILURE;

    pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    if (!tokenStream)
        return EXIT_FAILURE;

    pMAliceParser parser = MAliceParserNew(tokenStream);
    if (!parser)
        return EXIT_FAILURE;

    MAliceParser_program_return progReturn = parser->program(parser);
    pANTLR3_COMMON_TREE_NODE_STREAM	nodes;

    if (parser->pParser->rec->state->errorCount > 0)
		  fprintf(stderr, "The parser returned %d errors, tree walking aborted.\n", parser->pParser->rec->state->errorCount);
    else {
        CompilerContext *ctx = new CompilerContext();
        
        treeWalker.validateTree(progReturn.tree, ctx);
		nodes	= antlr3CommonTreeNodeStreamNewTree(progReturn.tree, ANTLR3_SIZE_HINT); // sIZE HINT WILL SOON BE DEPRECATED!!
        
        delete ctx;
	}
    
    // Cleanup
    parser->free(parser), parser = NULL;
    tokenStream->free (tokenStream), tokenStream = NULL;
    lexer->free(lexer), lexer = NULL;
    input->close(input), input = NULL;

    return EXIT_SUCCESS;
}