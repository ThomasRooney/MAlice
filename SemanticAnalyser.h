
#ifndef _MALICESEMANTICANALYSIS
#define _MALICESEMANTICANALYSIS

#include <iostream>

#include "CompilerContext.h"
#include "Types.h"

namespace MAlice {

    class SemanticAnalyser {
    private:
        ASTNode m_tree;
        
        void validateCompilerContext(CompilerContext *ctx);
    public:
        SemanticAnalyser(ASTNode tree);
        void validate();
    }; // class SemanticAnalyser
    
}; // namespace MAlice

#endif /* #ifndef _MALICESEMANTICANALYSIS */
