
#include "SemanticAnalyser.h"

#include "ASTWalker.h"
#include "Entity.h"
#include "ErrorFactory.h"
#include "Utilities.h"

namespace MAlice {

    SemanticAnalyser::SemanticAnalyser(ASTNode tree, CompilerContext *ctx)
    {
        m_tree = tree;
        m_compilerContext = ctx;
    }
    
    bool SemanticAnalyser::validateAndGenerateIR(llvm::Module **IRModule)
    {
        ASTWalker treeWalker = ASTWalker();
        llvm::Module *module = new llvm::Module("root module", llvm::getGlobalContext());
        
        if (!treeWalker.validateTree(m_tree, m_compilerContext))
            return false;
        
        if (!validateCompilerContext(m_compilerContext))
            return false;
        
        if (IRModule)
            *IRModule = module;
        
        return true;
    }
    
    bool SemanticAnalyser::validateCompilerContext(CompilerContext *ctx)
    {
        Entity *hattaEntity = NULL;
        
        if (!ctx->isSymbolInScope("hatta", &hattaEntity)) {
            m_compilerContext->getErrorReporter()->reportError(ErrorFactory::createSemanticError("Entry point procedure hatta() is not declared."));            
            return false;
        } else {
            MAliceEntityType type = Utilities::getTypeOfEntity(hattaEntity);
            
            if (type != MAliceEntityTypeProcedure) {
                if (type == MAliceEntityTypeFunction)
                    m_compilerContext->getErrorReporter()->reportError(ErrorFactory::createSemanticError("Entry point hatta() is declared as a function. It should be declared as a procedure."));
                else {
                    // By virtue of the fact that hatta is a keyword, we shouldn't hit this case, as it can't be declared as a variable.
                    m_compilerContext->getErrorReporter()->reportError(ErrorFactory::createSemanticError("Symbol 'hatta' is declared as a " + hattaEntity->humanReadableName() + ". It should be declared as a procedure."));
                }
                
                return false;
            }
        }
        
        return true;
    }

}; // namespace MAlice