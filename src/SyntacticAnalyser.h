
#ifndef _MALICESYNTACTICANALYSER
#define _MALICESYNTACTICANALYSER

#include <iostream>

#include "ErrorReporter.h"
#include "MAliceLexer.h"
#include "MAliceParser.h"
#include "Types.h"

namespace MAlice {

    class SyntacticAnalyser {
    private:
        std::string m_filePath;
        pMAliceLexer m_lexer;
        pMAliceParser m_parser;
        pANTLR3_INPUT_STREAM m_input;
        pANTLR3_COMMON_TOKEN_STREAM m_tokenStream;
        ErrorReporter *m_errorReporter;
    public:
        SyntacticAnalyser(std::string filePath);
        ~SyntacticAnalyser();
        
        ASTNode parsedInput();
        
        void setErrorReporter(ErrorReporter *errorReporter);
    }; // class SyntacticAnalyser
    
} // namespace MAlice

#endif /* #ifndef _MALICESYNTACTICANALYSER */
