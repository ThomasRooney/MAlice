#include "MAliceLexer.h"
#include "MAliceParser.h"

#ifdef _WIN32
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned long int uint64_t;
#endif


// Skeleton file based on http://stackoverflow.com/a/8542203
int main(int argc,int *argv[])
{
    pANTLR3_UINT8      fName;
    uint8_t *bufferData;
    uint32_t bufferSize;
    pANTLR3_UINT8 bufferName;
    pANTLR3_INPUT_STREAM       input;

    if (argc < 2 || argv[1] == NULL)
    {
        fName   =(pANTLR3_UINT8)"C:\\Code\\MaliceFinal\\malice\\input"; // Note in VS2005 debug, working directory must be configured
    }
    else
    {
        fName   = (pANTLR3_UINT8)argv[1];
    }

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

    parser->program(parser);

    return 0;
}
