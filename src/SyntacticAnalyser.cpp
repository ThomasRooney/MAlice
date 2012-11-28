
#include "SyntacticAnalyser.h"

#include "CompilerContext.h"
#include "ErrorFactory.h"

namespace MAlice {
    
    SyntacticAnalyser::SyntacticAnalyser(CompilerContext *compilerContext)
    {
        m_compilerContext = compilerContext;
    }
    
    bool SyntacticAnalyser::parseInput(std::string filePath, ASTNode *outTree)
    {
        pANTLR3_UINT8 path;
        path = (pANTLR3_UINT8)filePath.c_str();
        pANTLR3_INPUT_STREAM inputStream = antlr3FileStreamNew(path, ANTLR3_ENC_UTF8);
        
        if (inputStream == NULL) {
            m_compilerContext->getErrorReporter()->reportError(ErrorFactory::createIOError("Could not open file '" + filePath + "'."));
            return false;
        }
        
        pMAliceLexer lexer = MAliceLexerNew(inputStream);
        if (!lexer)
            return false;
        
        pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
        if (!tokenStream)
            return false;
        
        m_compilerContext->setInputStream(inputStream);
        m_compilerContext->setLexer(lexer);
        m_compilerContext->setTokenStream(tokenStream);
        
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