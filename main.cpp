#include "MAliceLexer.h"
#include "MAliceParser.h"
#include <string>


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



// Skeleton file based on http://stackoverflow.com/a/8542203
int main(int argc,char *argv[])
{
    pANTLR3_UINT8      fName;
    uint8_t *bufferData;
    uint32_t bufferSize;
    pANTLR3_UINT8 bufferName;
    pANTLR3_INPUT_STREAM       input;
      char *path=NULL;
      size_t size = NULL;
      path=getcwd(path,size);
      if (path == 0)
        perror("getcwd error");
      std::string spath = std::string(path);
      spath.append("\\");
    if (argc < 2 || argv[1] == NULL)
    {
      spath.append("input");
      
    }
    else
    {
        spath.append(argv[1]);
    }
    fName   = (pANTLR3_UINT8)spath.c_str();

     // Create the input stream using the supplied file name
     // (Use antlr38BitFileStreamNew for UTF16 input).
     //
    input  = antlr3FileStreamNew(fName, ANTLR3_ENC_UTF8);

    if (!input)
        return 1;

    pMAliceLexer lexer = MAliceLexerNew(input);
    if (!lexer)
        return 1;

    pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    if (!tokenStream)
        return 1;

    pMAliceParser parser = MAliceParserNew(tokenStream);
    if (!parser)
        return 1;

    MAliceParser_program_return progReturn = parser->program(parser);
    pANTLR3_COMMON_TREE_NODE_STREAM	nodes;

    // Finally, when the parser runs, it will produce an AST that can be traversed by the 
    // the tree parser: c.f. SimpleCWalker.g3t
    //
    if (parser->pParser->rec->state->errorCount > 0)
    {
		  fprintf(stderr, "The parser returned %d errors, tree walking aborted.\n", parser->pParser->rec->state->errorCount);
    }
    else
	  {
		printf("Tree : %s\n", progReturn.tree->toStringTree(progReturn.tree)->chars);
		nodes	= antlr3CommonTreeNodeStreamNewTree(progReturn.tree, ANTLR3_SIZE_HINT); // sIZE HINT WILL SOON BE DEPRECATED!!

		// Tree parsers are given a common tree node stream (or your override)
		//
		//treePsr	= SimpleCWalkerNew(nodes);

		//treePsr->program(treePsr);
		//nodes->free(nodes); nodes = NULL;
		//treePsr ->free  (treePsr);	    treePsr	= NULL;
	}

    // We did not return anything from this parser rule, so we can finish. It only remains
    // to close down our open objects, in the reverse order we created them
    //
    
   // psr	    ->free  (psr);	    psr		= NULL;
   // /tstream ->free  (tstream);	    tstream	= NULL;
   // lxr	    ->free  (lxr);	    lxr		= NULL;
   // input   ->close (input);	    input	= NULL;

    return 0;
}