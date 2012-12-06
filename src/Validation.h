
#ifndef _MALICEVALIDATION
#define _MALICEVALIDATION

#include <iostream>
#include "Types.h"

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;
    
    namespace Validation {
        
        
        // Separator comment (for git).
        
        bool validateArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        bool validateVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        
        
    } // namespace Validation
} // namespace MAlice


#endif /* #ifndef _MALICEVALIDATION */
