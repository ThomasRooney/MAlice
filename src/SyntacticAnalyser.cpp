
#include "SyntacticAnalyser.h"

#include "CompilerContext.h"

namespace MAlice {
    
    SyntacticAnalyser::SyntacticAnalyser(std::string filePath, CompilerContext *compilerContext)
    {
        m_compilerContext = compilerContext;
        
        pANTLR3_UINT8 path;
        path = (pANTLR3_UINT8)filePath.c_str();
        pANTLR3_INPUT_STREAM inputStream = antlr3FileStreamNew(path, ANTLR3_ENC_UTF8);
        
        if (inputStream == NULL)
            compilerContext->getErrorReporter()->reportError(ErrorType::IO, "No Input File Specified", true);
        
        pMAliceLexer lexer = MAliceLexerNew(inputStream);
        if (!lexer)
            return;
        
        pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
        if (!tokenStream)
            return;
        
        compilerContext->setInputStream(inputStream);
        compilerContext->setLexer(lexer);
        compilerContext->setTokenStream(tokenStream);
    }
    
    bool SyntacticAnalyser::parseInput(ASTNode *outTree)
    {
        pMAliceParser parser = MAliceParserNew(m_compilerContext->getTokenStream());
        if (!parser)
            return false;
        
        if (parser->pParser->rec->state->errorCount > 0)
            return false;
        
        MAliceParser_program_return ast = parser->program(parser);
        if (outTree != NULL)
            *outTree = ast.tree;
        
        if (m_compilerContext->getErrorReporter()->hasReportedErrors())
            return false;
        
        return true;
    }
    
}; // namespace MAlice