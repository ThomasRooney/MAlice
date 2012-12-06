
#ifndef _MALICEVALIDATION
#define _MALICEVALIDATION

#include <iostream>
#include "Types.h"

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;
    
    namespace Validation {
        bool validateAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validatePrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        // Separator comment (for git).
        
        bool validateVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        
        
    } // namespace Validation
} // namespace MAlice


#endif /* #ifndef _MALICEVALIDATION */
