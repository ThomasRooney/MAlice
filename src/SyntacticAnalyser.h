
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
        CompilerContext *m_compilerContext;
        
    public:
        SyntacticAnalyser(std::string filePath, CompilerContext *compilerContext);
        
        bool parseInput(ASTNode *outTree);
    }; // class SyntacticAnalyser
    
} // namespace MAlice

#endif /* #ifndef _MALICESYNTACTICANALYSER */
