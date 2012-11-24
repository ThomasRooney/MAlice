
#ifndef _MALICESEMANTICANALYSIS
#define _MALICESEMANTICANALYSIS

#include <iostream>

#include "ErrorReporter.h"
#include "Types.h"

namespace MAlice {
    
    class CompilerContext;

    class SemanticAnalyser {
    private:
        ASTNode m_tree;
        CompilerContext *m_compilerContext;
        
        void validateCompilerContext(CompilerContext *ctx);

    public:
        SemanticAnalyser(ASTNode tree, CompilerContext *ctx);
        void validate();
        
        void setErrorReporter(ErrorReporter *errorReporter);
    }; // class SemanticAnalyser
    
}; // namespace MAlice

#endif /* #ifndef _MALICESEMANTICANALYSIS */
