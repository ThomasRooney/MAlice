
#include "SyntacticAnalyser.h"

#include "CompilerContext.h"

namespace MAlice {
    
    SyntacticAnalyser::SyntacticAnalyser(std::string filePath, CompilerContext *compilerContext)
    {
        m_compilerContext = compilerContext;
        
        pANTLR3_UINT8 path;
        m_parser = NULL;
        path = (pANTLR3_UINT8)filePath.c_str();
        m_input = antlr3FileStreamNew(path, ANTLR3_ENC_UTF8);
        
        if (m_input == NULL)
          compilerContext->getErrorReporter()->reportError(ErrorTypeIO, "No Input File Specified", true);
        
        m_lexer = MAliceLexerNew(m_input);
        if (!m_lexer)
            return;
        
        m_tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(m_lexer));
        if (!m_tokenStream)
            return;
    }
    
    SyntacticAnalyser::~SyntacticAnalyser()
    {
        if (m_parser)
            m_parser->free(m_parser), m_parser = NULL;
        
        if (m_tokenStream)
            m_tokenStream->free(m_tokenStream), m_tokenStream = NULL;
        
        if (m_lexer)
            m_lexer->free(m_lexer), m_lexer = NULL;
        
        if (m_input)
            m_input->close(m_input), m_input = NULL;
    }
    
    ASTNode SyntacticAnalyser::parsedInput()
    {
        
        pMAliceParser parser = MAliceParserNew(m_tokenStream);
        if (!parser)
            return NULL;
        
        if (parser->pParser->rec->state->errorCount > 0)
            return NULL;
        
        MAliceParser_program_return ast = parser->program(parser);
        
        return ast.tree;
    }
    
}; // namespace MAlice