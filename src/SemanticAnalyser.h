
#ifndef _MALICESEMANTICANALYSIS
#define _MALICESEMANTICANALYSIS

#include "ErrorReporter.h"
#include "Types.h"

namespace MAlice {
    
    class CompilerContext;

    class SemanticAnalyser {
    private:
        ASTNode m_tree;
        CompilerContext *m_compilerContext;
        
        bool validateCompilerContext(CompilerContext *ctx);

    public:
        SemanticAnalyser(ASTNode tree, CompilerContext *ctx);
        bool validateAndGenerateIR(llvm::Module **IRModule);
        bool validateAST();
        bool generateIR(llvm::Module **IRModule);
        
        void setErrorReporter(ErrorReporter *errorReporter);
    }; // class SemanticAnalyser
    
}; // namespace MAlice

#endif /* #ifndef _MALICESEMANTICANALYSIS */
