
#ifndef _MALICEVALIDATION
#define _MALICEVALIDATION

#include <iostream>
#include "Types.h"

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;
    
    namespace Validation {
        bool validateAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateArbitraryBlockNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateBodyNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validatePrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateParamsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        
        
    } // namespace Validation
} // namespace MAlice


#endif /* #ifndef _MALICEVALIDATION */
