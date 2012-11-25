
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
        ASTWalker treeWalker = ASTWalker();
        
        treeWalker.validateTree(m_tree, m_compilerContext);
        validateCompilerContext(m_compilerContext);
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