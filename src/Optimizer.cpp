#include "Optimizer.h"
#include "Utilities.h"


namespace MAlice {


        Optimizer::Optimizer (llvm::Module* module)
        {
            m_module = module;
            
        }

        void Optimizer::constantFoldingPass(ASTNode node, CompilerContext *ctx)
        {   
            // Is node interesting to the constantFolder
            switch(Utilities::getNodeType(node))
            {
            case VARDECLARATION:
                // Take a note of this variable.
                break;
            case PLUS:
            case MINUS:
                break;
            default:
                {
                    for (int i = 0; i < Utilities::getNumberOfChildNodes(node); i++)
                        constantFoldingPass(Utilities::getChildNodeAtIndex(node,i), ctx);
                }
            }
//            If all subexpressions are constants, we replace the whole expression with a new constant computed from the sub-constants.
//            Otherwise, at least one of the subexpressions is not a constant, even after simplification. We now check whether at least one of them has changed, i.e., it or one of its descendants has been simplified. If this is the case, we must recreate a new similar expression with the new subexpressions.
//            Else (no subexpression has changed, and at least one subexpression is not a constant), we return the original expression.
        }
    
        llvm::Module *Optimizer::getModule()
        {
            return m_module;
        }
}