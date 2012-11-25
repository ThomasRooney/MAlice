
#include "SemanticAnalyser.h"

#include "ASTWalker.h"

namespace MAlice {

    SemanticAnalyser::SemanticAnalyser(ASTNode tree, CompilerContext *ctx)
    {
        m_tree = tree;
        m_compilerContext = ctx;
    }
    
    void SemanticAnalyser::validate()
    {
        CompilerContext *ctx = new CompilerContext();
        
        ASTWalker treeWalker = ASTWalker();
        
        treeWalker.validateTree(m_tree, ctx);
        validateCompilerContext(ctx);
        
        delete ctx;
    }
    
    void SemanticAnalyser::validateCompilerContext(CompilerContext *ctx)
    {
        if (!ctx->isSymbolInScope("hatta", NULL)) {
            m_compilerContext->getErrorReporter()->reportError(ErrorType::Semantic,
                                                               "Entry point procedure hatta() is not declared.",
                                                               false);
        }
    }

}; // namespace MAlice