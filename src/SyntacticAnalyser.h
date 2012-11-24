
#ifndef _MALICESYNTACTICANALYSER
#define _MALICESYNTACTICANALYSER

#include <iostream>

#include "ErrorReporter.h"
#include "MAliceLexer.h"
#include "MAliceParser.h"
#include "Types.h"

namespace MAlice {
    
    class CompilerContext;

    class SyntacticAnalyser {
    private:
        std::string m_filePath;
        pMAliceLexer m_lexer;
        pMAliceParser m_parser;
        pANTLR3_INPUT_STREAM m_input;
        pANTLR3_COMMON_TOKEN_STREAM m_tokenStream;
        CompilerContext *m_compilerContext;
        
    public:
        SyntacticAnalyser(std::string filePath, CompilerContext *compilerContext);
        ~SyntacticAnalyser();
        
        ASTNode parsedInput();
    }; // class SyntacticAnalyser
    
} // namespace MAlice

#endif /* #ifndef _MALICESYNTACTICANALYSER */
