
#ifndef _MALICEVALIDATION
#define _MALICEVALIDATION

#include <iostream>
#include "Types.h"

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;
    
    namespace Validation {
        validateAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        validateIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
        validateDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        // Separator comment (for git).
        
        bool validateVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        
        
    } // namespace Validation
} // namespace MAlice


#endif /* #ifndef _MALICEVALIDATION */
