
#ifndef _MALICESEMANTICANALYSIS
#define _MALICESEMANTICANALYSIS

#include <iostream>

#include "CompilerContext.h"
#include "ErrorReporter.h"
#include "Types.h"

namespace MAlice {

    class SemanticAnalyser {
    private:
        ASTNode m_tree;
        ErrorReporter *m_errorReporter;
        
        void validateCompilerContext(CompilerContext *ctx);
    public:
        SemanticAnalyser(ASTNode tree);
        void validate();
        
        void setErrorReporter(ErrorReporter *errorReporter);
    }; // class SemanticAnalyser
    
}; // namespace MAlice

#endif /* #ifndef _MALICESEMANTICANALYSIS */
