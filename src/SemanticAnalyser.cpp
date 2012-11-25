
#include "SemanticAnalyser.h"

#include "ASTWalker.h"

namespace MAlice {

    SemanticAnalyser::SemanticAnalyser(ASTNode tree, CompilerContext *ctx)
    {
        m_tree = tree;
        m_compilerContext = ctx;
    }
    
    bool SemanticAnalyser::validate()
    {
        ASTWalker treeWalker = ASTWalker();
        
        if (!treeWalker.validateTree(m_tree, m_compilerContext))
            return false;
        
        if (!validateCompilerContext(m_compilerContext))
            return false;
        
        return true;
    }
    
    bool SemanticAnalyser::validateCompilerContext(CompilerContext *ctx)
    {
        if (!ctx->isSymbolInScope("hatta", NULL)) {
            m_compilerContext->getErrorReporter()->reportError(ErrorType::Semantic,
                                                               "Entry point procedure hatta() is not declared.",
                                                               false);
            
            return false;
        }
        
        return true;
    }

}; // namespace MAlice