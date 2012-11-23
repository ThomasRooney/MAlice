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
    pANTLR3_UINT8 fName;
    pANTLR3_INPUT_STREAM input;
    
    ASTWalker treeWalker = ASTWalker();

    char *path = NULL;
    size_t size = (size_t)NULL;
    path=getcwd(path,size);
    if (path == 0)
    perror("getcwd error");
    std::string spath = std::string(path);
    spath.append("\\");
    
    if (argc < 2 || argv[1] == NULL) {
      spath.append("input");
    }
    else {
        spath.append(argv[1]);
    }
    
    fName   = (pANTLR3_UINT8)spath.c_str();
    input  = antlr3FileStreamNew(fName, ANTLR3_ENC_UTF8);

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
    {
		  fprintf(stderr, "The parser returned %d errors, tree walking aborted.\n", parser->pParser->rec->state->errorCount);
    }
    else {


//		printf("Tree : %s\n", progReturn.tree->toStringTree(progReturn.tree)->chars);
        treeWalker.validateTree(progReturn.tree );
		nodes	= antlr3CommonTreeNodeStreamNewTree(progReturn.tree, ANTLR3_SIZE_HINT); // sIZE HINT WILL SOON BE DEPRECATED!!

		// Tree parsers are given a common tree node stream (or your override)
		//
		//treePsr	= SimpleCWalkerNew(nodes);

		//treePsr->program(treePsr);
		//nodes->free(nodes); nodes = NULL;
		//treePsr ->free  (treePsr);	    treePsr	= NULL;
	}
    
    // Cleanup
    parser->free(parser), parser = NULL;
    tokenStream->free (tokenStream), tokenStream = NULL;
    lexer->free(lexer), lexer = NULL;
    input->close(input), input = NULL;

    return EXIT_SUCCESS;
}