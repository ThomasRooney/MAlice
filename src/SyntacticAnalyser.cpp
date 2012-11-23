
#include "SyntacticAnalyser.h"

namespace MAlice {
    
    SyntacticAnalyser::SyntacticAnalyser(std::string filePath)
    {
        pANTLR3_UINT8 path;
        
        path = (pANTLR3_UINT8)filePath.c_str();
        m_input = antlr3FileStreamNew(path, ANTLR3_ENC_UTF8);
        
        m_lexer = MAliceLexerNew(m_input);
        if (!m_lexer)
            return;
        
        m_tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(m_lexer));
        if (!m_tokenStream)
            return;
    }
    
    SyntacticAnalyser::~SyntacticAnalyser()
    {
        m_parser->free(m_parser), m_parser = NULL;
        m_tokenStream->free(m_tokenStream), m_tokenStream = NULL;
        m_lexer->free(m_lexer), m_lexer = NULL;
        m_input->close(m_input), m_input = NULL;
    }
    
    ASTNode SyntacticAnalyser::parsedInput()
    {
        
        pMAliceParser parser = MAliceParserNew(m_tokenStream);
        if (!parser)
            return NULL;
        
        MAliceParser_program_return ast = parser->program(parser);
        
        if (parser->pParser->rec->state->errorCount > 0)
            return NULL;
        
        return ast.tree;
    }
    
}; // namespace MAlice