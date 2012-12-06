
#ifndef _MALICEVALIDATION
#define _MALICEVALIDATION

#include <iostream>

namespace MAlice {
    namespace Validation {
        validateAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        validateIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
        validateDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        // Separator comment (for git).
        
        
    } // namespace Validation
} // namespace MAlice


#endif /* #ifndef _MALICEVALIDATION */
