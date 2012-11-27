
#ifndef _MALICESEMANTICCHECKS
#define _MALICESEMANTICCHECKS

#include <iostream>
#include <list>

#include "Types.h"

namespace MAlice {
    
    class FunctionProcedureEntity;
    class ParameterEntity;
  
    bool checkSymbolNotInCurrentScopeOrOutputError(std::string identifier, ASTNode node, CompilerContext *ctx);
    bool checkValidInvocationNode(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkCompatibleFunctionInvocationReturnType(ASTNode invocationNode, MAliceType expectedType, ASTWalker *walker, CompilerContext *ctx);
    
    bool visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(ASTNode node, FunctionProcedureEntity *entity, ASTWalker *walker, CompilerContext *ctx);
    std::list<ParameterEntity> getParameterTypesFromParamsNode(ASTNode paramsNode);
    bool checkExpression(ASTNode node, ASTWalker *walker, CompilerContext *ctx, MAliceType type);
    bool checkIsValidInvocationAndReturnType(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm);
    MAliceType getReturnTypeAndCheckIsValidInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    MAliceType getTypeFromExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    
    bool checkValidAssignmentStatementNode(ASTNode assignmentStatementNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkValidIfStatementNode(ASTNode ifStatementNode, ASTWalker *walker, CompilerContext *ctx);
    
};

#endif /* #ifndef _MALICESEMANTICCHECKS */
