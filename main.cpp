#include "MAliceLexer.h"
#include "MAliceParser.h"

// Skeleton file based on http://stackoverflow.com/a/8542203
int main()
{
    uint8_t *bufferData;
    uint32_t bufferSize;
    pANTLR3_UINT8 bufferName;

    pANTLR3_INPUT_STREAM input = antlr3StringStreamNew(bufferData, ANTLR3_ENC_8BIT, bufferSize, bufferName);
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
