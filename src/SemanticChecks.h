
#ifndef _MALICESEMANTICCHECKS
#define _MALICESEMANTICCHECKS

#include <iostream>
#include <list>

#include "Types.h"

namespace MAlice {
    
    class FunctionProcedureEntity;
    class ParameterEntity;
    class Type;
    
    namespace Validation {
  
        bool checkSymbolNotInCurrentScopeOrOutputError(std::string identifier, ASTNode node, CompilerContext *ctx);
        bool checkCompatibleFunctionInvocationReturnType(ASTNode invocationNode, MAlice::Type expectedType, ASTWalker *walker, CompilerContext *ctx);
        
        bool checkSymbolForInvocationIsValidOrOutputError(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        bool checkReturnTypeForInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        bool checkNumberOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        bool checkTypesOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        bool checkHasReturnValueInAllExecutionPaths(ASTNode bodyNode);
        bool checkExpression(ASTNode node, ASTWalker *walker, CompilerContext *ctx, Type type);
        bool checkExpression(ASTNode node, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, MAlice::Type type);
        
        bool checkValidAssignmentStatementNode(ASTNode assignmentStatementNode, ASTWalker *walker, CompilerContext *ctx);
        bool checkValidIfStatementNode(ASTNode ifStatementNode, ASTWalker *walker, CompilerContext *ctx);
        bool checkValidPrintStatementNode(ASTNode printStatementNode, ASTWalker *walker, CompilerContext *ctx);
        bool checkValidReturnStatementNode(ASTNode returnStatementNode, ASTWalker *walker, CompilerContext *ctx);
        
        bool checkReturnValueForAllExecutionPaths(ASTNode bodyNode, ASTWalker *walker, CompilerContext *ctx);
        bool checkReturnTypeForInvocation(ASTNode invocationNode, MAlice::Type type, ASTWalker *walker, CompilerContext *ctx);
        
    }
};

#endif /* #ifndef _MALICESEMANTICCHECKS */
